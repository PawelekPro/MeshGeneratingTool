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
* File      : MGTMesh_MeshObject.hpp
* Author    : Paweł Gilewicz
* Date      : 25/01/2025
*/

#ifndef MGTMesh_MeshObject_H
#define MGTMesh_MeshObject_H

#include <vtkMultiBlockDataSet.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

class MGTMesh_MeshObject : public vtkMultiBlockDataSet {
public:
	static MGTMesh_MeshObject* New();
	vtkTypeMacro(MGTMesh_MeshObject, vtkMultiBlockDataSet);

	MGTMesh_MeshObject();
	~MGTMesh_MeshObject() override;

	void SetInternalMesh(vtkSmartPointer<vtkUnstructuredGrid> mesh);
	void SetBoundaryMesh(vtkSmartPointer<vtkPolyData> mesh);

	vtkSmartPointer<vtkUnstructuredGrid> GetInternalMesh() const;
	vtkSmartPointer<vtkPolyData> GetBoundaryMesh() const;

private:
	vtkSmartPointer<vtkUnstructuredGrid> _internalMesh;
	vtkSmartPointer<vtkPolyData> _boundaryMesh;
};

#endif
