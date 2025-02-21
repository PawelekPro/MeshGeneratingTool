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
* File      : NetgenPlugin_Netgen2VTK.h
* Author    : Paweł Gilewicz
* Date      : 23/11/2024
*/
#ifndef NETGENPLUGIN_NETGEN2VTK_H
#define NETGENPLUGIN_NETGEN2VTK_H

#include "NetgenPlugin_Defs.hpp"

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

class MGTMesh_MeshObject;

namespace netgen {
class Mesh;
}

class NETGENPLUGIN_EXPORT NetgenPlugin_Netgen2VTK {
public:
	NetgenPlugin_Netgen2VTK(const netgen::Mesh& netgenMesh);

public:
	void ConvertToInternalMesh(MGTMesh_MeshObject* mesh);
	void ConvertToBoundaryMesh(MGTMesh_MeshObject* mesh);

private:
	vtkPoints* PopulateMeshNodes();

private:
	const netgen::Mesh& _netgenMesh;
};

#endif