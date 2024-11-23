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
* Date      : 21/11/2024
*/

#include "NetgenPlugin_Mesher.h"
#include "MGTMesh.h"
#include "NetgenPlugin_MeshInfo.h"
#include "NetgenPlugin_Netgen2VTK.h"
#include "NetgenPlugin_NetgenLibWrapper.h"
#include "NetgenPlugin_Parameters.h"

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_B3d.hxx>
#include <Bnd_Box.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Triangulation.hxx>
#include <Standard_Failure.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#ifndef OCCGEOMETRY
#define OCCGEOMETRY
#endif
#include <meshing.hpp>
#include <occgeom.hpp>

namespace netgen {
NETGENPLUGIN_DLL_HEADER
extern MeshingParameters mparam;
}

TopTools_IndexedMapOfShape ShapesWithLocalSize;
std::map<int, double> VertexId2LocalSize;
std::map<int, double> EdgeId2LocalSize;
std::map<int, double> FaceId2LocalSize;
std::map<int, double> SolidId2LocalSize;

//----------------------------------------------------------------------------
void updateTriangulation(const TopoDS_Shape& shape) {

	try {
		BRepMesh_IncrementalMesh e(shape, 0.01, true);
	} catch (Standard_Failure&) {
	}
}

//----------------------------------------------------------------------------
NetgenPlugin_Mesher::NetgenPlugin_Mesher(
	MGTMesh* mesh, const TopoDS_Shape& shape, const bool isVolume)
	: _mesh(mesh)
	, _shape(shape)
	, _isVolume(isVolume)
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

	if (_isVolume) {
		mparams.quad = false;
	} else {
		// FIXME: add parameters for surface meshing
		mparams.quad = false;
	}

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
bool NetgenPlugin_Mesher::ComputeMesh() {
	std::cout << "Netgen mesh compute event requested" << std::endl;
	NetgenPlugin_NetgenLibWrapper ngLib;
	netgen::MeshingParameters& mparams = netgen::mparam;

	netgen::OCCGeometry occgeo;
	std::cout << "Preparing geometry" << std::endl;
	this->PrepareOCCgeometry(occgeo, _shape);
	_occgeom = &occgeo;

	_ngMesh = nullptr;
	NetgenPlugin_MeshInfo initState;
	int err = 0;

	mparams.maxh = 10.0;
	if (mparams.maxh == 0.0)
		mparams.maxh = occgeo.boundingbox.Diam();

	if (mparams.minh == 0.0 && _fineness != NetgenPlugin_Parameters::UserDefined)
		mparams.minh = this->GetDefaultMinSize(_shape, mparams.maxh);

	std::cout << mparams << std::endl;
	occgeo.face_maxh = mparams.maxh;
	int startWith = netgen::MESHCONST_ANALYSE;
	int endWith = netgen::MESHCONST_MESHSURFACE;

	std::cout << "Starting mesh generation process" << std::endl;
	try {
		err = ngLib.GenerateMesh(occgeo, startWith, endWith, _ngMesh);
		std::cout << err << std::endl;
	} catch (Standard_Failure& ex) {
		std::cout << "OpenCASCADE Exception: " << ex.GetMessageString() << std::endl;
	} catch (netgen::NgException& ex) {
		std::cout << "Netgen Exception: " << ex.What() << std::endl;
	}

	if (!_ngMesh)
		return false;
	ngLib.setMesh((nglib::Ng_Mesh*)_ngMesh);

	std::cout << _ngMesh->GetNP() << std::endl;

	if (!mparams.uselocalh)
		_ngMesh->LocalHFunction().SetGrading(mparams.grading);

	NetgenPlugin_Netgen2VTK netgen2vtk = NetgenPlugin_Netgen2VTK(*_ngMesh);
	_mesh->SetInternalMesh(netgen2vtk.GetInternalMesh());
	_mesh->SetBoundaryMesh(netgen2vtk.GetBoundaryMesh());

	return !err;
}

//----------------------------------------------------------------------------
double NetgenPlugin_Mesher::GetDefaultMinSize(
	const TopoDS_Shape& geom, const double maxSize) {
	updateTriangulation(geom);

	TopLoc_Location loc;
	int i1, i2, i3;
	double minh = 1e100;
	Bnd_B3d bb;
	TopExp_Explorer fExp(geom, TopAbs_FACE);

	for (; fExp.More(); fExp.Next()) {
		Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(
			TopoDS::Face(fExp.Current()), loc);
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