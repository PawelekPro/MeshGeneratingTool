/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

*=============================================================================
* File      : NetgenPlugin_Mesher.cpp
* Author    : Paweł Gilewicz
* Date      : 06/04/2025
*/
#include "NetgenPlugin_Mesher.hpp"
#include "MGTMeshUtils_ComputeError.hpp"
#include "MGTMeshUtils_ControlPoint.h"
#include "MGTMeshUtils_DefaultParameters.hpp"
#include "MGTMeshUtils_ProgressReporter.hpp"
#include "MGTMesh_Algorithm.hpp"
#include "MGTMesh_MeshObject.hpp"
#include "NetgenPlugin_MeshInfo.h"
#include "NetgenPlugin_Netgen2VTK.h"
#include "NetgenPlugin_NetgenLibWrapper.h"
#include "NetgenPlugin_Parameters.hpp"

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <Standard_Failure.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Iterator.hxx>
#include <gp_Pnt.hxx>
#include <gp_XYZ.hxx>

#ifndef OCCGEOMETRY
#define OCCGEOMETRY
#endif
#include <meshing.hpp>
#include <occgeom.hpp>

#include <spdlog/spdlog.h>

#include <map>
#include <set>
#include <vector>

namespace netgen {
NETGENPLUGIN_DLL_HEADER
extern MeshingParameters mparam;
}

TopTools_IndexedMapOfShape ShapesWithLocalSize;
std::map<int, double> VertexId2LocalSize;
std::map<int, double> EdgeId2LocalSize;
std::map<int, double> FaceId2LocalSize;
std::map<int, double> SolidId2LocalSize;

std::vector<MGTMeshUtils::ControlPoint> ControlPoints;
std::set<int> ShapesWithControlPoints;

//----------------------------------------------------------------------------
void setLocalSize(const TopoDS_Shape& GeomShape, double LocalSize) {
	if (GeomShape.IsNull())
		return;
	TopAbs_ShapeEnum GeomType = GeomShape.ShapeType();
	if (GeomType == TopAbs_COMPOUND) {
		for (TopoDS_Iterator it(GeomShape); it.More(); it.Next()) {
			setLocalSize(it.Value(), LocalSize);
		}
		return;
	}
	int key;
	if (!ShapesWithLocalSize.Contains(GeomShape))
		key = ShapesWithLocalSize.Add(GeomShape);
	else
		key = ShapesWithLocalSize.FindIndex(GeomShape);

	if (GeomType == TopAbs_VERTEX) {
		VertexId2LocalSize[key] = LocalSize;
	} else if (GeomType == TopAbs_EDGE) {
		EdgeId2LocalSize[key] = LocalSize;
	} else if (GeomType == TopAbs_FACE) {
		FaceId2LocalSize[key] = LocalSize;
	} else if (GeomType == TopAbs_SOLID) {
		SolidId2LocalSize[key] = LocalSize;
	}
	return;
}

//----------------------------------------------------------------------------
void setLocalSize(const TopoDS_Edge& edge, double size, netgen::Mesh& mesh,
	const bool overrideMinH = true) {
	if (size <= std::numeric_limits<double>::min())
		return;

	Standard_Real u1, u2;
	Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, u1, u2);

	if (curve.IsNull()) {
		TopoDS_Iterator vIt(edge);
		if (!vIt.More())
			return;
		gp_Pnt p = BRep_Tool::Pnt(TopoDS::Vertex(vIt.Value()));
		NetgenPlugin_Mesher::RestrictLocalSize(
			mesh, p.XYZ(), size, overrideMinH);

	} else {
		const int nb = (int)(1.5 * MGTMesh_Algorithm::EdgeLength(edge) / size);
		Standard_Real delta = (u2 - u1) / nb;

		for (int i = 0; i < nb; i++) {
			Standard_Real u = u1 + delta * i;
			gp_Pnt p = curve->Value(u);
			NetgenPlugin_Mesher::RestrictLocalSize(
				mesh, p.XYZ(), size, overrideMinH);
			netgen::Point3d pi(p.X(), p.Y(), p.Z());
			double resultSize = mesh.GetH(pi);

			if (resultSize - size > 0.1 * size)
				// netgen does restriction iff oldH/newH > 1.2 (localh.cpp:136)
				NetgenPlugin_Mesher::RestrictLocalSize(
					mesh, p.XYZ(), resultSize / 1.201, overrideMinH);
		}
	}
}

//----------------------------------------------------------------------------
NetgenPlugin_Mesher::NetgenPlugin_Mesher(MGTMesh_MeshObject* mesh,
	const TopoDS_Shape& shape, const NetgenPlugin_Parameters* algorithm)
	: _mesh(mesh)
	, _shape(shape)
	, _algorithm(algorithm)
	, _optimize(true)
	, _fineness(NetgenPlugin_Parameters::GetDefaultFineness())
	, _isViscousLayers2D(false)
	, _ngMesh(nullptr)
	, _occGeom(nullptr)
	, _selfPtr(nullptr) {

	SPDLOG_INFO("Initializing NetgenPlugin_Mesher object");

	this->SetMeshParameters();
	ShapesWithLocalSize.Clear();
	VertexId2LocalSize.clear();
	EdgeId2LocalSize.clear();
	FaceId2LocalSize.clear();
	SolidId2LocalSize.clear();
	ControlPoints.clear();
	ShapesWithControlPoints.clear();
}

//----------------------------------------------------------------------------
NetgenPlugin_Mesher::NetgenPlugin_Mesher(const TopoDS_Shape& shape)
	: _mesh(nullptr)
	, _shape(shape)
	, _algorithm(nullptr)
	, _optimize(true)
	, _fineness(NetgenPlugin_Parameters::GetDefaultFineness())
	, _isViscousLayers2D(false)
	, _ngMesh(nullptr)
	, _occGeom(nullptr)
	, _selfPtr(nullptr) { }

//----------------------------------------------------------------------------
NetgenPlugin_Mesher::~NetgenPlugin_Mesher() {
	if (_selfPtr) {
		*_selfPtr = nullptr;
	}
	_selfPtr = nullptr;
	_ngMesh = nullptr;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::SetMeshParameters() {
	netgen::MeshingParameters& mParams = netgen::mparam;
	mParams = netgen::MeshingParameters();

	mParams.maxh = _algorithm->maxSize;
	mParams.minh = _algorithm->minSize;

	mParams.segmentsperedge = _algorithm->nbSegPerEdge;
	mParams.grading = _algorithm->growthRate;

	mParams.curvaturesafety = _algorithm->nbSegPerEdge;
	mParams.secondorder = _algorithm->secondOrder;

	mParams.quad = _algorithm->quadAllowed;

	_fineness = -_algorithm->fineness;
	mParams.uselocalh = _algorithm->surfaceCurvature;

	_optimize = _algorithm->optimize;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::PrepareOCCGeometry(
	netgen::OCCGeometry& occGeom, const TopoDS_Shape& shape) {
	occGeom.shape = shape;
	occGeom.changed = 1;
	occGeom.BuildFMap();
	occGeom.BuildVisualizationMesh(0.01);
	occGeom.CalcBoundingBox();
	// occgeom.PrintNrShapes();
}

int NetgenPlugin_Mesher::ComputeProgress() {
	const std::string task = netgen::multithread.task;
	// spdlog::debug("Current task: {}", task);

	if (!_occGeom || !_algorithm)
		return 0;

	if (!_algorithm->Is3DAlgortihm()) {
		int numFacesMeshed = 0;
		const int totalFaces = _occGeom->fmap.Extent();

		for (auto it = _occGeom->fmap.cbegin(); it != _occGeom->fmap.cend();
			++it) {
			const int faceId = _occGeom->fmap.FindIndex(*it);
			if (_occGeom->facemeshstatus[faceId] >= 1)
				++numFacesMeshed;
		}

		if (totalFaces == 0)
			return 0;

		_surfaceMeshProgress
			= static_cast<int>(100.0 * numFacesMeshed / totalFaces);
		return Min(_surfaceMeshProgress, 99);
	}

	if (task == MGTMeshUtils_NetgenTasks::SurfaceMeshing) {
		int numFacesMeshed = 0;
		const int totalFaces = _occGeom->fmap.Extent();

		for (auto it = _occGeom->fmap.cbegin(); it != _occGeom->fmap.cend();
			++it) {
			const int faceId = _occGeom->fmap.FindIndex(*it);
			if (_occGeom->facemeshstatus[faceId] >= 1)
				++numFacesMeshed;
		}

		if (totalFaces == 0)
			return 0;

		_surfaceMeshProgress
			= static_cast<int>(50.0 * numFacesMeshed / totalFaces);

		return Min(_surfaceMeshProgress, 49);
	}

	if (task == MGTMeshUtils_NetgenTasks::SurfaceOptimizing) {
		return 49;
	}

	if (task == MGTMeshUtils_NetgenTasks::VolumeMeshing) {
		return _volumeMeshProgress;
	}

	if (task.find(MGTMeshUtils_NetgenTasks::VolumeOptimizing)
		!= std::string::npos) {
		_volumeMeshProgress = 99;
		return _volumeMeshProgress;
	}

	return 50;
};

//----------------------------------------------------------------------------
int NetgenPlugin_Mesher::ComputeMesh(
	const std::function<void(int)>& progressCallback,
	const std::function<void(const std::string&)>& statusCallback) {
	MGTMeshUtils_ProgressReporter progressReporter(
		500, [this]() { return this->ComputeProgress(); }, progressCallback);

	NetgenPlugin_NetgenLibWrapper ngLib;
	netgen::MeshingParameters& mParams = netgen::mparam;
	netgen::multithread.terminate = 0;

	CALL_STATUS(statusCallback, "Preparing geometry...");
	netgen::OCCGeometry occGeom;
	SPDLOG_INFO("Preparing geometry...");
	NetgenPlugin_Mesher::PrepareOCCGeometry(occGeom, _shape);
	_occGeom = &occGeom;

	_ngMesh = nullptr;
	NetgenPlugin_MeshInfo initState;
	int err = MGTMeshUtils_ComputeErrorName::COMPERR_OK;

	if (mParams.maxh == 0.0)
		mParams.maxh = occGeom.boundingbox.Diam();

	if (mParams.minh == 0.0
		&& _fineness != NetgenPlugin_Parameters::UserDefined)
		mParams.minh = MGTMeshUtils_DefaultParameters::GetDefaultMinSize(
			_shape, mParams.maxh);

	SPDLOG_INFO("Mesh input parameters:");
	std::cout << mParams << std::endl;

	occGeom.face_maxh = mParams.maxh;

	int startWith = netgen::MESHCONST_ANALYSE;
	int endWith = netgen::MESHCONST_ANALYSE;

	SPDLOG_INFO("Starting mesh generation process");
	try {
		CALL_STATUS(statusCallback, "Analyse mesh...");
		err = NetgenPlugin_NetgenLibWrapper::GenerateMesh(
			occGeom, startWith, endWith, _ngMesh);

		if (netgen::multithread.terminate)
			return MGTMeshUtils_ComputeErrorName::COMPERR_CANCELED;

	} catch (Standard_Failure& ex) {
		SPDLOG_ERROR("OpenCASCADE Exception: {}", ex.GetMessageString());
	} catch (netgen::NgException& ex) {
		SPDLOG_ERROR("Netgen Exception: {}", ex.What());
	}

	if (!_ngMesh)
		return err;
	ngLib.setMesh(reinterpret_cast<nglib::Ng_Mesh*>(_ngMesh));

	if (err)
		return err;

	// if (!mParams.uselocalh)
	// 	_ngMesh->LocalHFunction().SetGrading(mParams.grading);

	// const TopoDS_Shape& shape = occgeo.fmap.FindKey(1);
	// setLocalSize(shape, 2);

	SetLocalSize(occGeom, *_ngMesh);

	// Compute 1D mesh
	startWith = endWith = netgen::MESHCONST_MESHEDGES;
	SPDLOG_INFO("Starting 1D mesh generation process");
	try {
		CALL_STATUS(statusCallback, "Computing 1D mesh...");
		err = ngLib.GenerateMesh(occGeom, startWith, endWith);

		if (netgen::multithread.terminate)
			return MGTMeshUtils_ComputeErrorName::COMPERR_CANCELED;

	} catch (Standard_Failure& ex) {
		SPDLOG_ERROR("OpenCASCADE Exception: {}", ex.GetMessageString());
	} catch (netgen::NgException& ex) {
		SPDLOG_ERROR("Netgen Exception: {}", ex.What());
	}

	if (err)
		return err;

	// Compute surface mesh
	mParams.uselocalh = true;
	startWith = netgen::MESHCONST_MESHSURFACE;
	endWith = _optimize ? netgen::MESHCONST_OPTSURFACE
						: netgen::MESHCONST_MESHSURFACE;

	SPDLOG_INFO("Starting surface mesh generation process");
	try {
		CALL_STATUS(statusCallback, "Computing surface mesh...");
		err = ngLib.GenerateMesh(occGeom, startWith, endWith);

		if (netgen::multithread.terminate)
			return MGTMeshUtils_ComputeErrorName::COMPERR_CANCELED;

	} catch (Standard_Failure& ex) {
		SPDLOG_ERROR("OpenCASCADE Exception: {}", ex.GetMessageString());
	} catch (netgen::NgException& ex) {
		SPDLOG_ERROR("Netgen Exception: {}", ex.What());
	}

	if (err)
		return err;

	CALL_STATUS(statusCallback, "Processing surface mesh...");
	const NetgenPlugin_Netgen2VTK netgen2vtk(*_ngMesh);
	netgen2vtk.ConvertToBoundaryMesh(_mesh);

	if (_algorithm->Is3DAlgortihm()) {
		startWith = netgen::MESHCONST_MESHVOLUME;
		endWith = _optimize ? netgen::MESHCONST_OPTVOLUME
							: netgen::MESHCONST_MESHVOLUME;
		SPDLOG_INFO("Starting volume mesh generation process");

		try {
			CALL_STATUS(statusCallback, "Computing volume mesh...");
			err = ngLib.GenerateMesh(occGeom, startWith, endWith);

			if (netgen::multithread.terminate)
				return MGTMeshUtils_ComputeErrorName::COMPERR_CANCELED;

		} catch (Standard_Failure& ex) {
			SPDLOG_ERROR("OpenCASCADE Exception: {}", ex.GetMessageString());
		} catch (netgen::NgException& ex) {
			SPDLOG_ERROR("Netgen Exception: {}", ex.What());
		}
	} else {
		return MGTMeshUtils_ComputeErrorName::COMPERR_OK;
	}

	if (err)
		return err;

	CALL_STATUS(statusCallback, "Processing volume mesh...");
	netgen2vtk.ConvertToInternalMesh(_mesh);
	return MGTMeshUtils_ComputeErrorName::COMPERR_OK;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::CancelMeshGeneration() {
	netgen::multithread.terminate = 1;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::RestrictLocalSize(netgen::Mesh& ngMesh,
	const gp_XYZ& p, double size, const bool overrideMinH) {
	if (size <= std::numeric_limits<double>::min())
		return;

	if (netgen::mparam.minh > size) {
		if (overrideMinH) {
			ngMesh.SetMinimalH(size);
			netgen::mparam.minh = size;
		} else {
			size = netgen::mparam.minh;
		}
	}
	const netgen::Point3d pi(p.X(), p.Y(), p.Z());
	ngMesh.RestrictLocalH(pi, size);
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::SetLocalSize(
	netgen::OCCGeometry& occGeom, netgen::Mesh& ngMesh) {
	// edges
	std::map<int, double>::const_iterator it;
	for (it = EdgeId2LocalSize.begin(); it != EdgeId2LocalSize.end(); ++it) {
		const int key = it->first;
		const double hi = it->second;
		const TopoDS_Shape& shape = ShapesWithLocalSize.FindKey(key);
		setLocalSize(TopoDS::Edge(shape), hi, ngMesh);
	}

	// vertices
	for (it = VertexId2LocalSize.begin(); it != VertexId2LocalSize.end();
		++it) {
		const int key = it->first;
		const double hi = it->second;
		const TopoDS_Shape& shape = ShapesWithLocalSize.FindKey(key);
		gp_Pnt p = BRep_Tool::Pnt(TopoDS::Vertex(shape));
		NetgenPlugin_Mesher::RestrictLocalSize(ngMesh, p.XYZ(), hi);
	}

	// faces
	for (it = FaceId2LocalSize.begin(); it != FaceId2LocalSize.end(); ++it) {
		int key = it->first;
		double val = it->second;
		const TopoDS_Shape& shape = ShapesWithLocalSize.FindKey(key);
		const int faceNgID = occGeom.fmap.FindIndex(shape);

		if (faceNgID >= 1) {
			occGeom.SetFaceMaxH(faceNgID, val, netgen::mparam);
			for (TopExp_Explorer edgeExp(shape, TopAbs_EDGE); edgeExp.More();
				edgeExp.Next()) {
				setLocalSize(TopoDS::Edge(edgeExp.Current()), val, ngMesh);
			}
		} else if (!ShapesWithControlPoints.count(key)) {
			std::cout << "Creating control points for face " << key
					  << std::endl;
			MGTMeshUtils::createPointsSampleFromFace(
				TopoDS::Face(shape), val, ControlPoints);
			ShapesWithControlPoints.insert(key);
		}
	}
	// ToDo: support for solids

	if (!ControlPoints.empty()) {
		std::cout << "Restricting local size for control point " << std::endl;
		for (size_t i = 0; i < ControlPoints.size(); ++i)
			NetgenPlugin_Mesher::RestrictLocalSize(
				ngMesh, ControlPoints[i].XYZ(), ControlPoints[i].Size());
	}
	return;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::SetParameters(
	const MGTMeshUtils_ViscousLayers* layersScheme) {
	_viscousLayers = layersScheme;
}