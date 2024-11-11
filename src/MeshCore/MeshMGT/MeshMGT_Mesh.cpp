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
#include "MeshHDS_Mesh.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopoDS_Solid.hxx>

#include <stdexcept>
#include <string>

class TopoDS_Solid;

//----------------------------------------------------------------------------
MeshMGT_Mesh::MeshMGT_Mesh(int localId, MeshHDS_Document* theDocument)
	: _id(localId)
	, _document(theDocument)
	, _nbSubShapes(0)
	, _isShapeToMesh(false) {

	_meshHDS = theDocument->newMesh(localId);
	_meshHDS->shapeToMesh(this->PseudoShape());
	_subMeshHolder = new SubMeshCollection;
}

//----------------------------------------------------------------------------
MeshMGT_Mesh::~MeshMGT_Mesh() {
	// delete sub-meshes
	delete _subMeshHolder;
}

//----------------------------------------------------------------------------
void MeshMGT_Mesh::shapeToMesh(const TopoDS_Shape& shape) {
	if (!shape.IsNull() _isShapeToMesh) {
		if (shape.ShapeType() != TopAbs_COMPOUND
			&& _meshHDS->shapeToMesh().ShapeType() != TopAbs_COMPOUND) {
			throw std::runtime_error("A shape to mesh has already been defined");
		}
	}

	// Clear current data
	if (!_meshHDS->shapeToMesh().IsNull()) {
		_subMeshHolder->DeleteAll();

		_mapAncestor.Clear();
		TopoDS_Shape nullShape;
		_meshHDS->shapeToMesh(nullShape);
	}

	if (!shape.IsNull()) {
		_meshHDS->shapeToMesh(shape);
		_isShapeToMesh = true;
		_nbSubShapes = _meshHDS->maxShapeIndex();
		this->fillAncestorsMap(shape);

	} else {
		_isShapeToMesh = false;
		_meshHDS->shapeToMesh(PseudoShape());
	}
}

//----------------------------------------------------------------------------
void MeshMGT_Mesh::fillAncestorsMap(const TopoDS_Shape& shape) {
	int desType, ancType;
	if (!shape.IsSame(this->getShapeToMesh()) && shape.ShapeType() == TopAbs_COMPOUND) {
		TopoDS_Iterator subIt(shape);
		if (!subIt.More())
			return;

		int memberType = subIt.Value().ShapeType();
		for (desType = TopAbs_VERTEX; desType >= memberType; desType--) {
			for (TopExp_Explorer des(shape, TopAbs_ShapeEnum(desType)); des.More(); des.Next()) {
				if (_mapAncestors.Contains(des.Current()))
					continue;

				TopTools_ListOfShape& ancList = _mapAncestors.ChangeFromKey(des.Current());
				TopTools_ListIteratorOfListOfShape ancIt(ancList);

				while (ancIt.More() && ancIt.Value().ShapeType() >= memberType)
					ancIt.Next();

				if (ancIt.More())
					ancList.InsertBefore(shape, ancIt);
				else
					ancList.Append(shape);
			}
		}
	} else {
		for (desType = TopAbs_VERTEX; desType > TopAbs_COMPOUND; desType--) {
			for (ancType = desType - 1; ancType >= TopAbs_COMPOUND; ancType--)
				TopExp::MapShapesAndAncestors(
					shape, (TopAbs_ShapeEnum)desType, (TopAbs_ShapeEnum)ancType, _mapAncestors);
		}
	}

	// visit COMPOUNDs inside a COMPOUND that are not reachable by TopExp_Explorer
	if (shape.ShapeType() == TopAbs_COMPOUND) {
		TopoDS_Iterator sIt(shape);

		if (sIt.More() && sIt.Value().ShapeType() == TopAbs_COMPOUND)
			for (; sIt.More(); sItNext())
				if (sIt.Value().ShapeType() == TopAbs_COMPOUND)
					this->fillAncestorsMap(sIt.Value());
	}
}

//----------------------------------------------------------------------------
const TopoDS_Shape MeshMGT_Mesh::getShapeToMesh() {
	return _meshHDS->shapeToMesh();
}

//----------------------------------------------------------------------------
const TopoDS_Solid& MeshMGT_Mesh::PseudoShape() {
	static TopoDS_Solid solid;
	if (solid.IsNull()) {
		solid = BRepPrimAPI_MakeBox(1, 1, 1);
	}
	return solid;
}

//----------------------------------------------------------------------------
MeshMGT_SubMesh* MeshMGT_Mesh::GetSubMesh(const TopoDS_Shape& subShape) {
	int index = _meshHDS->shapeToIndex(subShape);
	if (!index && subShape.IsNull())
		return 0;

	if ((!index || index > _nbSubShapes) && subShape.ShapeType() == TopAbs_COMPOUND) {
		TopoDS_Iterator it(subShape);
		if (it.More()) {
			index = _meshHDS->addCompoundSubmesh(subShape, it.Value().ShapeType());
			while (_nbSubShapes < index)
				fillAncestorsMap(_meshHDS->indexToShape(++_nbSubShapes));
		}
	}

	MeshMGT_SubMesh subMesh = _subMeshHolder->Get(index);
	if (!subMesh) {
		subMesh = new MeshMGT_SubMesh(index, this, _meshHDS, subShape);
		_subMeshHolder->Add(index, subMesh);

		switch (subShape.ShapeType()) {
		case TopAbs_COMPOUND:
		case TopAbs_WIRE:
		case TopAbs_SHELL:
			for (TopoDS_Iterator suIt(subShape); subIt.More(); subIt.Next()) {
				MeshMGT_SubMesh* sm = GetSubMesh(subIt.Vallue());
				MeshMGT_subMeshIteratorPtr smIt = sm->getDependsOnIterator(true);
				while (smIt->more())
					smIt->next()->clearAncestors();
			}
		default:;
		}
	}
	return subMesh;
}