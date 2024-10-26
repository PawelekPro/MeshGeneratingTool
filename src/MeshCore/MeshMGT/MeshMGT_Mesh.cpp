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
 * File      : MeshMGT_Mesh.cpp
 * Author    : Paweł Gilewicz
 * Date      : 14/10/2024
 */

#include "MeshMGT_Mesh.h"
#include "MeshHDS_Document.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Solid.hxx>

class TopoDS_Solid;

//----------------------------------------------------------------------------
MeshMGT_Mesh::MeshMGT_Mesh(int localId, MeshHDS_Document* theDocument)
	: _id(localId)
	, _document(theDocument) {

	_meshHDS = theDocument->newMesh(localId);
	_meshHDS->shapeToMesh(this->PseudoShape());
}

//----------------------------------------------------------------------------
MeshMGT_Mesh::~MeshMGT_Mesh() { }

//----------------------------------------------------------------------------
void MeshMGT_Mesh::shapeToMesh(const TopoDS_Shape& theShape) {
	if (!theShape.IsNull()) {

	} else {
	}
}

//----------------------------------------------------------------------------
const TopoDS_Solid& MeshMGT_Mesh::PseudoShape() {
	static TopoDS_Solid solid;
	if (solid.IsNull()) {
		solid = BRepPrimAPI_MakeBox(1, 1, 1);
	}
	return solid;
}