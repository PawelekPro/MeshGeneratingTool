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
* File      : MGTMesh_Scheme.cpp
* Author    : Paweł Gilewicz
* Date      : 25/01/2025
*/

#include "MGTMesh_Scheme.hpp"

//----------------------------------------------------------------------------
MGTMesh_Scheme::MGTMesh_Scheme(int schemeId)
	: _schemeId(schemeId)
	, _engineLib(NETGEN)
	, _type(ALG_3D) {
	_name = "generic";
	_shapeType = 0;
	_alg_dim = -1;
}

//----------------------------------------------------------------------------
const char* MGTMesh_Scheme::GetName() const { return _name.c_str(); }

//----------------------------------------------------------------------------
int MGTMesh_Scheme::GetID() const { return _schemeId; }

//----------------------------------------------------------------------------
int MGTMesh_Scheme::GetType() const { return _type; }

//----------------------------------------------------------------------------
bool MGTMesh_Scheme::Is3DAlgortihm() const { return _type == ALG_3D; }

//----------------------------------------------------------------------------
bool MGTMesh_Scheme::Is2DAlgortihm() const { return _type == ALG_2D; }

//----------------------------------------------------------------------------
bool MGTMesh_Scheme::Is1DAlgortihm() const { return _type == ALG_1D; }

//----------------------------------------------------------------------------
void MGTMesh_Scheme::SetType(MGTMesh_Scheme::SchemeType type) { _type = type; }

//----------------------------------------------------------------------------
void MGTMesh_Scheme::SetDim(int algDim) { _alg_dim = algDim; }

//----------------------------------------------------------------------------
void MGTMesh_Scheme::SetShapeType(int shapeType) { _shapeType = shapeType; }

//----------------------------------------------------------------------------
int MGTMesh_Scheme::GetDim() const {
	int dim = 0;
	switch (_type) {
	case ALG_1D:
		dim = 1;
		break;
	case ALG_2D:
		dim = 2;
		break;
	case ALG_3D:
		dim = 3;
		break;
	case ALG_0D:
		dim = 0;
		break;
	case PARAM_ALG:
		dim = (_alg_dim < 0) ? -_alg_dim : _alg_dim;
		break;
	}

	return dim;
}

//----------------------------------------------------------------------------
int MGTMesh_Scheme::GetShapeType() const { return _shapeType; }

//----------------------------------------------------------------------------
int MGTMesh_Scheme::GetEngineLib() const { return _engineLib; }

//----------------------------------------------------------------------------
void MGTMesh_Scheme::SetEngineLib(MGTMesh_Scheme::Engine libType) { _engineLib = libType; }
