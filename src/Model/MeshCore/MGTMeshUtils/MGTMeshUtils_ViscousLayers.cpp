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
* File      : MGTMeshUtils_ViscousLayers.cpp
* Author    : Paweł Gilewicz
* Date      : 27/11/2024
*/

#include "MGTMeshUtils_ViscousLayers.h"

MGTMeshUtils_ViscousLayers::MGTMeshUtils_ViscousLayers(int schemeId)
	: MGTMesh_Scheme(schemeId)
	, _isToIgnoreShapes(1)
	, _nbLayers(1)
	, _thickness(1)
	, _stretchFactor(1)
	, _method(SURF_OFFSET_SMOOTH)
	, _groupName("") {
	_name = MGTMeshUtils_ViscousLayers::GetSchemeType();
	_alg_dim = -3;
}