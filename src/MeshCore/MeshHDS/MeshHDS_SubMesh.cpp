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
 *
 * NetgenPlugin : C++ implementation
 * File      	: MeshHDS_SubMesh.cpp
 * Author    	: Paweł Gilewicz
 * Date      	: 27/10/2024
 */

#include "MeshHDS_SubMesh.h"

#include "MeshHDS_Mesh.h"

//----------------------------------------------------------------------------
MeshHDS_SubMesh::MeshHDS_SubMesh(const MeshHDS_Mesh* parent, int index)
	: MeshDS_ElementHolder(parent) {
	_parent = parent;
	_index = index;
}

//----------------------------------------------------------------------------
MeshHDS_SubMesh::~MeshHDS_SubMesh() { }