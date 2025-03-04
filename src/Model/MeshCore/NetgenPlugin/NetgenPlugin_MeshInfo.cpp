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
* File      : NetgenPlugin_MeshInfo.h
* Author    : Paweł Gilewicz
* Date      : 23/11/2024
*/

#include "NetgenPlugin_MeshInfo.h"
#include "NetgenPlugin_NetgenLibWrapper.h"

#ifndef OCCGEOMETRY
#define OCCGEOMETRY
#endif
#include <meshing.hpp>
#include <occgeom.hpp>

//----------------------------------------------------------------------------
NetgenPlugin_MeshInfo::NetgenPlugin_MeshInfo(
	netgen::Mesh* ngMesh, bool checkRemovedElems)
	: _elementsRemoved(false)
	, _copyOfLocalH(nullptr) {
	if (ngMesh) {
		_nbNodes = ngMesh->GetNP();
		_nbSegments = ngMesh->GetNSeg();
		_nbFaces = ngMesh->GetNSE();
		_nbVolumes = ngMesh->GetNE();
	}

	if (checkRemovedElems) {
		for (int i = 1; i <= ngMesh->GetNSE() && !_elementsRemoved; ++i) {
			_elementsRemoved = ngMesh->SurfaceElement(i).IsDeleted();
		}
	} else {
		_nbNodes = _nbSegments = _nbFaces = _nbVolumes = 0;
	}
}

//----------------------------------------------------------------------------
void NetgenPlugin_MeshInfo::transferLocalH(
	netgen::Mesh* fromMesh, netgen::Mesh* toMesh) {
	if (!fromMesh->LocalHFunctionGenerated()) {
		return;
	}

	if (!toMesh->LocalHFunctionGenerated()) {
		NetgenPlugin_NetgenLibWrapper::CalcLocalH(toMesh);
	}

	const size_t size = sizeof(netgen::LocalH);
	_copyOfLocalH = new char[size];
	memcpy((void*)_copyOfLocalH, (void*)&toMesh->LocalHFunction(), size);
	memcpy((void*)&toMesh->LocalHFunction(), (void*)&fromMesh->LocalHFunction(), size);
}

//----------------------------------------------------------------------------
void NetgenPlugin_MeshInfo::restoreLocalH(netgen::Mesh* toMesh) {
	if (_copyOfLocalH) {
		const size_t size = sizeof(netgen::LocalH);
		memcpy((void*)&toMesh->LocalHFunction(), (void*)_copyOfLocalH, size);
		delete[] _copyOfLocalH;
		_copyOfLocalH = 0;
	}
}