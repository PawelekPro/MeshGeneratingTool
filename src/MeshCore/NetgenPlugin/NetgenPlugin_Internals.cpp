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
 * File      	: NetgenPlugin_Internals.cpp
 * Author    	: Paweł Gilewicz
 * Date      	: 27/10/2024
 */

#include "NetgenPlugin_Internals.h"
#include "MeshHDS_Mesh.h"
#include "MeshMGT_Mesh.h"
#include "MeshMGT_SubMesh.h"

#include "TopExp_Explorer.hxx"
#include "TopoDS_Shape.hxx"

//----------------------------------------------------------------------------
NetgenPlugin_Internals::NetgenPlugin_Internals(
	MeshMGT_Mesh& mesh, const TopoDS_Shape& shape, bool is3D)
	: _mesh(mesh)
	, _is3D(is3D) {
	MeshHDS_Mesh* meshHDS = mesh.GetMeshHDS();

	TopExp_Explorer face, edge;
	for (face.Init(shape, TopAbs_FACE); face.More(); face.Next()) {
		int faceID = meshHDS->shapeToIndex(face.Current());

		// Find not computed internal edges

		for (edge.Init(face.Current().Oriented(TopAbs_FORWARD), TopAbs_EDGE); edge.More(); edge.Next()) {
			if (edge.Current().Orientation() == TopAbs_INTERNAL) {
				MeshMGT_SubMesh* edgeSubMesh = mesh.GetSubMesh(edge.Current();) if (edgeSubMesh->IsE)
			}
		}
	}
}