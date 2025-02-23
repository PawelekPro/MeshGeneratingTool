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
* File      : GMSHPlugin_Mesher.cpp
* Author    : Paweł Gilewicz
* Date      : 23/02/2025
*/

#include "GMSHPlugin_Mesher.hpp"
#include "GMSHPlugin_Parameters.hpp"

#include "MGTMeshUtils_ComputeError.hpp"

#ifdef _WIN32
#include <gmsh.h_cwrap>
#endif

#ifdef linux
#include <gmsh.h>
#include <gmshc.h>
#endif

#include <spdlog/spdlog.h>

#include <map>

//----------------------------------------------------------------------------
GMSHPlugin_Mesher::GMSHPlugin_Mesher(
	MGTMesh_MeshObject* mesh, const TopoDS_Shape& shape, const GMSHPlugin_Parameters* algorithm)
	: _mesh(mesh)
	, _shape(shape)
	, _algorithm(algorithm) {

	SPDLOG_INFO("Initializing GMSHPlugin_Mesher object");
}

//----------------------------------------------------------------------------
void GMSHPlugin_Mesher::SetGmshOptions() {
	SPDLOG_INFO("Setting GMSH Options");

	std::map<int, double> mapAlgo2d;
	mapAlgo2d[0] = 2; // !Automatic
	mapAlgo2d[1] = 1; // !MeshAdapt
	mapAlgo2d[2] = 5; // !Delaunay
	mapAlgo2d[3] = 6; // !Frontal-Delaunay
	mapAlgo2d[4] = 8; // !DelQuad (Frontal-Delaunay for Quads)
	mapAlgo2d[5] = 9; // !Packing of parallelograms
	mapAlgo2d[6] = 11; // !Quasistructured quads with cross-fields

	std::map<int, double> mapAlgo3d;
	mapAlgo3d[0] = 1; // !Delaunay
	mapAlgo3d[1] = 4; // !Frontal
	mapAlgo3d[2] = 7; // !MMG3D
	mapAlgo3d[3] = 9; // !R-tree
	mapAlgo3d[4] = 10; // !HXT

	gmsh::option::setNumber("Mesh.Algorithm", mapAlgo2d[_algorithm->Get2DAlgo()]);
	gmsh::option::setNumber("Mesh.Algorithm3D", mapAlgo3d[_algorithm->Get3DAlgo()]);
	gmsh::option::setNumber(
		"Mesh.RecombinationAlgorithm", mapAlgo3d[_algorithm->GetRecomb2DAlgo()]);
	gmsh::option::setNumber("Mesh.SubdivisionAlgorithm", (double)_algorithm->GetSubdivAlgo());
	gmsh::option::setNumber("Mesh.RemeshAlgorithm", (double)_algorithm->GetRemeshAlgo());
	gmsh::option::setNumber("Mesh.RemeshParametrization", (double)_algorithm->GetRemeshPara());
	gmsh::option::setNumber("Mesh.Smoothing", _algorithm->GetSmoothSteps());
}

//----------------------------------------------------------------------------
int GMSHPlugin_Mesher::ComputeMesh() {
	int err = MGTMeshUtils_ComputeErrorName::COMPERR_OK;

	gmsh::initialize();

	return err;
}