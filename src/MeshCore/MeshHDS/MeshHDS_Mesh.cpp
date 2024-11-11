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
#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>

//----------------------------------------------------------------------------
MeshHDS_Mesh::MeshHDS_Mesh(int theMeshID)
	: _subMeshHolder(new SubMeshCollection) {
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
const TopoDS_Shape MeshHDS_Mesh::shapeToMesh() {
	return _shape;
}

//----------------------------------------------------------------------------
const int MeshHDS_Mesh::shapeToIndex(const TopoDS_Shape& shape) {
	return _indexToShape.FindIndex(shape);
}

//----------------------------------------------------------------------------
const TopoDS_Shape& MeshHDS_Mesh::indexToShape(int shapeIndex) {
	try {
		if (shapeIndex > 0)
			return _indexToShape.FindKey(shapeIndex);
	} catch (...) { }

	static TopoDS_Shape nullShape;
	return nullShape;
}

//----------------------------------------------------------------------------
const MeshHDS_SubMesh* MeshHDS_Mesh::meshElements(const TopoDS_Shape& shape) {
	int index = this->shapeToIndex(shape);
	return (MeshHDS_SubMesh*)(index ? _subMeshHolder->Get(index) : 0);
}

//----------------------------------------------------------------------------
const MeshHDS_SubMesh* MeshHDS_Mesh::meshElements(const int index) {
	return const_cast<MeshHDS_SubMesh*>(_subMeshHolder->Get(index));
}

//----------------------------------------------------------------------------
int MeshHDS_Mesh::addCompoundSubmesh(const TopoDS_Shape& shape, TopAbs_ShapeEnum type) {
	int mainIndex = 0;
	if (isGroupOfSubShapes(shape)) {
		mainIndex = _indexToShape.Add(shape);
		bool all = (type == TopAbs_SHAPE);
		if (all) {
			mainIndex = -mainIndex;
		}

		MeshHDS_SubMesh* nsm = newSubMesh(mainIndex);
		if (!nsm->isComplexSubmesh()) {
			int shapeType = Max(TopAbs_SOLID, all ? _shape.ShapeType() : type);
			int typeLimit = all ? TopAbs_VERTEX : type;

			for (; shapeType < +typeLimit; shapeType++) {
				TopExp_Explorer exp(shape, TopAbs_ShapeEnum(shapeType));
				for (; exp.More(); exp.Next()) {
					int index = _indexToShape.FindIndex(exp.Current());
					if (index) {
						nsm->addSubMesh(this->newSubMesh(index));
					}
				}
			}
		}
	}
	return mainIndex;
}

//----------------------------------------------------------------------------
const bool MeshHDS_Mesh::isGroupOfSubShapes(const TopoDS_Shape& shape) {
	if (_indexToShape.Contains(shape)) {
		return true;
	}

	for (TopoDS_Iterator it(shape); it.More(); it.Next()) {
		if (isGroupOfSubShapes(it.Value())) {
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------
MeshHDS_SubMesh* MeshHDS_Mesh::newSubMesh(int index) {
	const MeshHDS_SubMesh* sm = meshElements(index);

	if (!sm) {
		sm = new MeshHDS_SubMesh(this, index);
		_subMeshHolder->Add(index, sm);
	}

	return const_cast<MeshHDS_SubMesh*>(sm);
}