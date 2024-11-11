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
 * File      	: MeshMGT_SubMesh.h
 * Author    	: Paweł Gilewicz
 * Date      	: 27/10/2024
 */
#ifndef MeshMGT_SUBMESH_H
#define MeshMGT_SUBMESH_H

#include "MeshDS_Iterator.hpp"
#include "MeshMGT_Macro.h"

#include <TopoDS_Shape.hxx>

#include <memory>

class MeshHDS_Mesh;
class MeshMGT_Mesh;
class MeshDS_Mesh;
class MeshHDS_SubMesh;
class MeshMGT_SubMesh;

typedef std::shared_ptr<MeshDS_Iterator<MeshMGT_SubMesh*>> MeshMGT_subMeshIteratorPtr;

class MeshMGT_EXPORT MeshMGT_SubMesh {
public:
	MeshMGT_SubMesh(
		int Id, MeshMGT_Mesh* parent, MeshHDS_Mesh* meshHDS, const TopoDS_Shape& subShape);
	virtual ~MeshMGT_SubMesh();

	MeshHDS_SubMesh* GetSubMeshDS();

	const MeshMGT_subMeshIteratorPtr getDependsOnIterator(
		const bool includeSelf, const bool reverse = false);

	void clearAncestors();

	const bool isEmpty();

protected:
	TopoDS_Shape _subShape;
	MeshHDS_SubMesh* _subMeshHDS;
	MeshMGT_Mesh* _parent;
	int _id;

	std::map<int, MeshMGT_SubMesh*> _mapDepend;
	bool _dependenceAnalysed;
	std::vector<MeshMGT_SubMesh*> _ancestors;
};

#endif