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
 * Mesh      : C++ implementation
 * File      : MeshHDS_Mesh.cpp
 * Author    : Paweł Gilewicz
 * Date      : 26/10/2024
 */

#include "MeshHDS_Mesh.h"

#include <TopExp.hxx>

//----------------------------------------------------------------------------
MeshHDS_Mesh::MeshHDS_Mesh(int theMeshID) {
	_persistentID = theMeshID;
}

//----------------------------------------------------------------------------
MeshHDS_Mesh::~MeshHDS_Mesh() { }

//----------------------------------------------------------------------------
void MeshHDS_Mesh::shapeToMesh(const TopoDS_Shape& shape) {
	_shape = shape;
	if (!shape.IsNull()) {
		TopExp::MapShapes(_shape, _indexToShape);
	}
}

//----------------------------------------------------------------------------
const int MeshHDS_Mesh::shapeToIndex(const TopoDS_Shape& shape) {
	return _indexToShape.FindIndex(shape);
}