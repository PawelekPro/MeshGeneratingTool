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
 * File      	: MeshMGT_SubMesh.cpp
 * Author    	: Paweł Gilewicz
 * Date      	: 27/10/2024
 */

#include "MeshMGT_SubMesh.h"

#include "MeshDS_IteratorsAux.hpp"

struct _Iterator : public MeshDS_Iterator<MeshMGT_SubMesh*> {
	_Iterator(MeshDS_Iterator<MeshMGT_SubMesh*>* subIt,
		MeshMGT_SubMesh* prepend,
		MeshMGT_SubMesh* append)
		: myAppend(append)
		, myIt(subIt) {
		myCur = prepend ? prepend : myIt->more() ? myIt->next()
												 : append;
		if (myCur == append)
			append = 0;
	}
	/// Return true if and only if there are other object in this iterator
	virtual bool more() {
		return myCur;
	}
	/// Return the current object and step to the next one
	virtual MeshMGT_SubMesh* next() {
		MeshMGT_SubMesh* res = myCur;
		if (myIt->more()) {
			myCur = myIt->next();
		} else {
			myCur = myAppend;
			myAppend = 0;
		}
		return res;
	}

	~_Iterator() { delete myIt; }

	MeshMGT_SubMesh *myAppend, *myCur;
	MeshDS_Iterator<MeshMGT_SubMesh*>* myIt;
};

//----------------------------------------------------------------------------
MeshMGT_SubMesh::MeshMGT_SubMesh(
	int Id, MeshMGT_Mesh* parent, MeshHDS_Mesh* meshHDS, const TopoDS_Shape& subShape)
	: _id(Id)
	, _parent(parent)
	, _subShape(subShape) {
	_subMeshHDS = meshHDS->meshElements(_subShape);
	_dependenceAnalysed = false;
}

//----------------------------------------------------------------------------
MeshMGT_SubMesh::~MeshMGT_SubMesh() { }

//----------------------------------------------------------------------------
const MeshMGT_subMeshIteratorPtr MeshMGT_SubMesh::getDependsOnIterator(
	const bool includeSelf, const bool reverse = false) {
	MeshMGT_SubMesh* me = (MeshMGT_SubMesh*)this;
	MeshMGT_SubMesh *preprend = 0, *append = 0;

	if (includeSelf) {
		if (reverse)
			prepend = me;
		else
			append = me;
	}
	typedef map<int, MeshMGT_SubMesh*> TMap;
	if (reverse) {
		return MeshMGT_subMeshIteratorPtr(
			new _Iterator(new MeshDS_mapReverseIterator<TMap>(me->DependsOn()), prepend, append));
	}
	{
		return MeshMGT_subMeshIteratorPtr(
			new _Iterator(new MeshDS_mapIterator<TMap>(me->DependsOn()), prepend, append));
	}
}

//----------------------------------------------------------------------------
void MeshMGT_SubMesh::clearAncestors() {
	_ancestors.clear();
}

MeshHDS_SubMesh* MeshMGT_SubMesh::GetSubMeshDS() {
	return _subMeshHDS ? _subMeshHDS : _subMeshHDS = _parent->GetMeshHDS()->meshElements(_subShape);
}

//----------------------------------------------------------------------------
const bool MeshMGT_SubMesh::isEmpty() {
	if (MeshHDS_SubMesh* smHDS = ((MeshMGT_SubMesh*)this)->GetSubMeshDS())
		return (!smHDS->Nb)
}