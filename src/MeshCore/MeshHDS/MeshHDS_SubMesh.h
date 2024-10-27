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
 * File      	: MeshHDS_SubMesh.h
 * Author    	: Paweł Gilewicz
 * Date      	: 27/10/2024
 */
#ifndef MeshHDS_SUBMESH_H
#define MeshHDS_SUBMESH_H

#include "MeshDS_ElementHolder.h"
#include "MeshHDS_Macro.h"

class MeshHDS_Mesh;
class MeshHDS_SubMesh;
typedef std::set<const MeshHDS_SubMesh*> TSubMeshSet;

class MeshHDS_EXPORT MeshHDS_SubMesh : public MeshDS_ElementHolder {
public:
	MeshHDS_SubMesh(const MeshHDS_Mesh* parent, int index);
	virtual ~MeshHDS_SubMesh();

private:
	int _index;
	const MeshHDS_Mesh* _parent;
	TSubMeshSet _subMeshes;
};

#endif