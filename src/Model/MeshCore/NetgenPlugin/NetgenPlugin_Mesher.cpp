/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
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
* Date      : 24/11/2024
*/

#include "NetgenPlugin_Mesher.hpp"
#include "MGTMeshUtils_ComputeError.hpp"
#include "MGTMeshUtils_ControlPoint.h"
#include "MGTMesh_Algorithm.hpp"
#include "MGTMesh_MeshObject.hpp"
#include "NetgenPlugin_MeshInfo.h"
#include "NetgenPlugin_Netgen2VTK.h"
#include "NetgenPlugin_NetgenLibWrapper.h"
#include "NetgenPlugin_Parameters.hpp"

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_B3d.hxx>
#include <Bnd_Box.hxx>
#include <Geom_Curve.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Triangulation.hxx>
#include <Standard_Failure.hxx>
#include <Standard_Real.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_MapOfShape.hxx>
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
void updateTriangulation(const TopoDS_Shape& shape) {

	try {
		BRepMesh_IncrementalMesh e(shape, 0.01, true);
	} catch (Standard_Failure&) { }
}

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
void setLocalSize(
	const TopoDS_Edge& edge, double size, netgen::Mesh& mesh, const bool overrideMinH = true) {
	if (size <= std::numeric_limits<double>::min())
		return;

	Standard_Real u1, u2;
	Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, u1, u2);

	if (curve.IsNull()) {
		TopoDS_Iterator vIt(edge);
		if (!vIt.More())
			return;
		gp_Pnt p = BRep_Tool::Pnt(TopoDS::Vertex(vIt.Value()));
		NetgenPlugin_Mesher::RestrictLocalSize(mesh, p.XYZ(), size, overrideMinH);

	} else {
		const int nb = (int)(1.5 * MGTMesh_Algorithm::EdgeLength(edge) / size);
		Standard_Real delta = (u2 - u1) / nb;

		for (int i = 0; i < nb; i++) {
			Standard_Real u = u1 + delta * i;
			gp_Pnt p = curve->Value(u);
			NetgenPlugin_Mesher::RestrictLocalSize(mesh, p.XYZ(), size, overrideMinH);
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
NetgenPlugin_Mesher::NetgenPlugin_Mesher(std::shared_ptr<MGTMesh_MeshObject> mesh,
	const TopoDS_Shape& shape, const NetgenPlugin_Parameters* algorithm)
	: _mesh(mesh)
	, _shape(shape)
	, _algorithm(algorithm)
	, _optimize(true)
	, _fineness(NetgenPlugin_Parameters::GetDefaultFineness())
	, _isViscousLayers2D(false)
	, _ngMesh(nullptr)
	, _occgeom(nullptr)
	, _selfPtr(nullptr) {

	std::cout << "Initializing NetgenPlugin_Mesher object" << std::endl;
	this->SetDefaultParameters();
	ShapesWithLocalSize.Clear();
	VertexId2LocalSize.clear();
	EdgeId2LocalSize.clear();
	FaceId2LocalSize.clear();
	SolidId2LocalSize.clear();
	ControlPoints.clear();
	ShapesWithControlPoints.clear();
}

//----------------------------------------------------------------------------
NetgenPlugin_Mesher::~NetgenPlugin_Mesher() {
	if (_selfPtr) {
		*_selfPtr = nullptr;
	}
	_selfPtr = 0;
	_ngMesh = nullptr;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::SetDefaultParameters() {
	netgen::MeshingParameters& mparams = netgen::mparam;
	mparams = netgen::MeshingParameters();

	//! maximal mesh edge size
	mparams.maxh = 0;
	mparams.minh = 0;
	//! minimal number of segments per edge
	mparams.segmentsperedge = NetgenPlugin_Parameters::GetDefaultNbSegPerEdge();
	//! rate of growth of size between elements
	mparams.grading = NetgenPlugin_Parameters::GetDefaultGrowthRate();
	//! safety factor for curvatures (elements per radius)
	mparams.curvaturesafety = NetgenPlugin_Parameters::GetDefaultNbSegPerRadius();
	//! create elements of second order
	mparams.secondorder = NetgenPlugin_Parameters::GetDefaultSecondOrder();

	// FIXME: add parameters for surface meshing
	mparams.quad = false;

	_fineness = NetgenPlugin_Parameters::GetDefaultFineness();
	mparams.uselocalh = NetgenPlugin_Parameters::GetDefaultSurfaceCurvature();
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::PrepareOCCgeometry(
	netgen::OCCGeometry& occgeom, const TopoDS_Shape& shape) {
	occgeom.shape = shape;
	occgeom.changed = 1;
	occgeom.BuildFMap();
	occgeom.BuildVisualizationMesh(0.01);
	occgeom.CalcBoundingBox();
	occgeom.PrintNrShapes();
}

//----------------------------------------------------------------------------
int NetgenPlugin_Mesher::ComputeMesh() {
	NetgenPlugin_NetgenLibWrapper ngLib;
	netgen::MeshingParameters& mparams = netgen::mparam;

	netgen::OCCGeometry occgeo;
	SPDLOG_INFO("Preparing geometry...");
	this->PrepareOCCgeometry(occgeo, _shape);
	_occgeom = &occgeo;

	_ngMesh = nullptr;
	NetgenPlugin_MeshInfo initState;
	int err = MGTMeshUtils_ComputeErrorName::COMPERR_OK;

	mparams.maxh = 100.0;
	if (mparams.maxh == 0.0)
		mparams.maxh = occgeo.boundingbox.Diam();

	if (mparams.minh == 0.0 && _fineness != NetgenPlugin_Parameters::UserDefined)
		mparams.minh = this->GetDefaultMinSize(_shape, mparams.maxh);

	std::cout << "Using mesh parametrs: " << mparams << std::endl;
	occgeo.face_maxh = mparams.maxh;
	int startWith = netgen::MESHCONST_ANALYSE;
	int endWith = netgen::MESHCONST_ANALYSE;

	SPDLOG_INFO("Starting mesh generation process");

	try {
		err = ngLib.GenerateMesh(occgeo, startWith, endWith, _ngMesh);
	} catch (Standard_Failure& ex) {
		SPDLOG_ERROR("OpenCASCADE Exception: {}", ex.GetMessageString());
	} catch (netgen::NgException& ex) {
		SPDLOG_ERROR("Netgen Exception: {}", ex.What());
	}

	if (!_ngMesh)
		return err;
	ngLib.setMesh((nglib::Ng_Mesh*)_ngMesh);

	if (err)
		return err;

	if (!mparams.uselocalh)
		_ngMesh->LocalHFunction().SetGrading(mparams.grading);

	// const TopoDS_Shape& shape = occgeo.fmap.FindKey(1);
	// setLocalSize(shape, 2);

	SetLocalSize(occgeo, *_ngMesh);

	// Compute 1D mesh
	startWith = endWith = netgen::MESHCONST_MESHEDGES;
	std::cout << "Starting 1D mesh generation process" << std::endl;
	try {
		err = ngLib.GenerateMesh(occgeo, startWith, endWith);
	} catch (Standard_Failure& ex) {
		SPDLOG_ERROR("OpenCASCADE Exception: {}", ex.GetMessageString());
	} catch (netgen::NgException& ex) {
		SPDLOG_ERROR("Netgen Exception: {}", ex.What());
	}

	if (err)
		return err;

	// Compute surface mesh
	mparams.uselocalh = true;
	startWith = netgen::MESHCONST_MESHSURFACE;
	endWith = _optimize ? netgen::MESHCONST_OPTSURFACE : netgen::MESHCONST_MESHSURFACE;
	std::cout << "Starting surface mesh generation process" << std::endl;

	try {
		err = ngLib.GenerateMesh(occgeo, startWith, endWith);
	} catch (Standard_Failure& ex) {
		SPDLOG_ERROR("OpenCASCADE Exception: {}", ex.GetMessageString());
	} catch (netgen::NgException& ex) {
		SPDLOG_ERROR("Netgen Exception: {}", ex.What());
	}

	if (err)
		return err;

	NetgenPlugin_Netgen2VTK netgen2vtk = NetgenPlugin_Netgen2VTK(*_ngMesh);
	netgen2vtk.ConvertToBoundaryMesh();
	_mesh->SetBoundaryMesh(netgen2vtk.GetBoundaryMesh());

	if (_algorithm->Is3DAlgortihm()) {
		// Compute volume mesh
		startWith = netgen::MESHCONST_MESHVOLUME;
		endWith = _optimize ? netgen::MESHCONST_OPTVOLUME : netgen::MESHCONST_MESHVOLUME;
		SPDLOG_INFO("Starting volume mesh generation process");

		try {
			err = ngLib.GenerateMesh(occgeo, startWith, endWith);
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

	netgen2vtk.ConvertToInternalMesh();
	_mesh->SetInternalMesh(netgen2vtk.GetInternalMesh());
	return MGTMeshUtils_ComputeErrorName::COMPERR_OK;
}

//----------------------------------------------------------------------------
double NetgenPlugin_Mesher::GetDefaultMinSize(const TopoDS_Shape& geom, const double maxSize) {
	updateTriangulation(geom);

	TopLoc_Location loc;
	int i1, i2, i3;
	double minh = 1e100;
	Bnd_B3d bb;
	TopExp_Explorer fExp(geom, TopAbs_FACE);

	for (; fExp.More(); fExp.Next()) {
		Handle(Poly_Triangulation) triangulation
			= BRep_Tool::Triangulation(TopoDS::Face(fExp.Current()), loc);
		if (triangulation.IsNull())
			continue;

		const double fTol = BRep_Tool::Tolerance(TopoDS::Face(fExp.Current()));
		const Standard_Integer numTriangles = triangulation->NbTriangles();

		for (Standard_Integer iT = 1; iT <= numTriangles; ++iT) {
			Poly_Triangle triangle = triangulation->Triangle(iT);
			triangle.Get(i1, i2, i3);

			gp_Pnt p1 = triangulation->Node(i1);
			gp_Pnt p2 = triangulation->Node(i2);
			gp_Pnt p3 = triangulation->Node(i3);

			// Calculate distances between vertices
			double dist2_1 = p1.SquareDistance(p2);
			double dist2_2 = p2.SquareDistance(p3);
			double dist2_3 = p3.SquareDistance(p1);

			// Check and update minimum distance
			if (dist2_1 < minh && fTol * fTol < dist2_1)
				minh = dist2_1;
			if (dist2_2 < minh && fTol * fTol < dist2_2)
				minh = dist2_2;
			if (dist2_3 < minh && fTol * fTol < dist2_3)
				minh = dist2_3;

			// Update bounding box
			bb.Add(p1);
			bb.Add(p2);
			bb.Add(p3);
		}
	}

	if (minh > 0.25 * bb.SquareExtent()) {
		minh = 1e-3 * sqrt(bb.SquareExtent());
	} else {
		minh = sqrt(minh);
	}

	if (minh > 0.5 * maxSize)
		minh = maxSize / 3.0;

	return minh;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::RestrictLocalSize(
	netgen::Mesh& ngMesh, const gp_XYZ& p, double size, const bool overrideMinH) {
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
	netgen::Point3d pi(p.X(), p.Y(), p.Z());
	ngMesh.RestrictLocalH(pi, size);
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::SetLocalSize(netgen::OCCGeometry& occgeo, netgen::Mesh& ngMesh) {
	// edges
	std::map<int, double>::const_iterator it;
	for (it = EdgeId2LocalSize.begin(); it != EdgeId2LocalSize.end(); it++) {
		int key = (*it).first;
		double hi = (*it).second;
		const TopoDS_Shape& shape = ShapesWithLocalSize.FindKey(key);
		setLocalSize(TopoDS::Edge(shape), hi, ngMesh);
	}

	// vertices
	for (it = VertexId2LocalSize.begin(); it != VertexId2LocalSize.end(); it++) {
		int key = (*it).first;
		double hi = (*it).second;
		const TopoDS_Shape& shape = ShapesWithLocalSize.FindKey(key);
		gp_Pnt p = BRep_Tool::Pnt(TopoDS::Vertex(shape));
		NetgenPlugin_Mesher::RestrictLocalSize(ngMesh, p.XYZ(), hi);
	}

	// faces
	for (it = FaceId2LocalSize.begin(); it != FaceId2LocalSize.end(); it++) {
		int key = (*it).first;
		double val = (*it).second;
		const TopoDS_Shape& shape = ShapesWithLocalSize.FindKey(key);
		int faceNgID = occgeo.fmap.FindIndex(shape);

		if (faceNgID >= 1) {
			occgeo.SetFaceMaxH(faceNgID, val, netgen::mparam);
			for (TopExp_Explorer edgeExp(shape, TopAbs_EDGE); edgeExp.More(); edgeExp.Next()) {
				setLocalSize(TopoDS::Edge(edgeExp.Current()), val, ngMesh);
			}
		} else if (!ShapesWithControlPoints.count(key)) {
			std::cout << "Creating control points for face " << key << std::endl;
			MGTMeshUtils::createPointsSampleFromFace(TopoDS::Face(shape), val, ControlPoints);
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
void NetgenPlugin_Mesher::SetParameters(const MGTMeshUtils_ViscousLayers* layersScheme) {
	_viscousLayers = layersScheme;
}