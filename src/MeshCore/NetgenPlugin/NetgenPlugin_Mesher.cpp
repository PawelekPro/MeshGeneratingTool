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
 */

#include "NetgenPlugin_Mesher.h"

#include "MeshMGT_Mesh.h"

// Netgen include files
#ifndef OCCGEOMETRY
#define OCCGEOMETRY
#endif
#include <meshing.hpp>
#include <occgeom.hpp>

namespace netgen {

NETGENPLUGIN_DLL_HEADER
extern MeshingParameters mparam;

NETGENPLUGIN_DLL_HEADER
extern volatile multithreadt multithread;

NETGENPLUGIN_DLL_HEADER
extern bool merge_solids;

// values used for occgeo.facemeshstatus
enum EFaceMeshStatus { FACE_NOT_TREATED = 0,
	FACE_FAILED = -1,
	FACE_MESHED_OK = 1,
};
}

//----------------------------------------------------------------------------
NetgenPlugin_Mesher::NetgenPlugin_Mesher(
	MeshMGT_Mesh* mesh, const TopoDS_Shape& shape, const bool isVolume)
	: _mesh(mesh)
	, _shape(shape)
	, _isVolume(isVolume) {
}

//----------------------------------------------------------------------------
NetgenPlugin_Mesher::~NetgenPlugin_Mesher() {
	_ngMesh = nullptr;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Mesher::setDefaultParameters() {
	netgen::MeshingParameters& mparams = netgen::mparam;
	mparams = netgen::MeshingParameters();
	mparams.maxh = 0;
	mparams.minh = 0;

#ifdef NETGEN_V6

	mparams.nthreads = std::thread::hardware_concurrency();

	// Environment variable to control multithreading
	if (getenv("MGT_NETGEN_DISABLE_MULTITHREADING")) {
		mparams.nthreads = 1;
		mparams.parallel_meshing = false;
	}

#endif
}