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
* File      : MGTMesh_ProxyMesh.hpp
* Author    : Paweł Gilewicz
* Date      : 08/02/2025
*/
#ifndef MGTMESH_PROXYMESH_HPP
#define MGTMESH_PROXYMESH_HPP

#include <vtkSmartPointer.h>

#include <memory>
#include <unordered_map>

class vtkActor;
class MGTMesh_MeshObject;

class MGTMesh_ProxyMesh {
public:
	MGTMesh_ProxyMesh(MGTMesh_MeshObject* mgtMesh);
	MGTMesh_ProxyMesh(std::unordered_map<int, MGTMesh_MeshObject*> meshObjectsMap);
	~MGTMesh_ProxyMesh();

	vtkSmartPointer<vtkActor> GetProxyMeshActor() const;

private:
	vtkSmartPointer<MGTMesh_MeshObject> _mgtMesh;
};

#endif