/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
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
* File      : MGTMesh_MeshObject.cpp
* Author    : Paweł Gilewicz
* Date      : 25/01/2025
*/
#include "MGTMesh_MeshObject.hpp"

vtkStandardNewMacro(MGTMesh_MeshObject);

//----------------------------------------------------------------------------
MGTMesh_MeshObject::MGTMesh_MeshObject()
	: _internalMesh(vtkSmartPointer<vtkUnstructuredGrid>::New())
	, _boundaryMesh(vtkSmartPointer<vtkPolyData>::New()) { }

//----------------------------------------------------------------------------
MGTMesh_MeshObject::~MGTMesh_MeshObject() {
	this->SetBlock(0, nullptr);
	this->SetBlock(1, nullptr);
}

//----------------------------------------------------------------------------
void MGTMesh_MeshObject::SetInternalMesh(vtkUnstructuredGrid* mesh) {
	_internalMesh = vtkSmartPointer<vtkUnstructuredGrid>::Take(mesh);
	this->SetBlock(0, _internalMesh);
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkUnstructuredGrid>
MGTMesh_MeshObject::GetInternalMesh() const {
	return _internalMesh;
}

//----------------------------------------------------------------------------
void MGTMesh_MeshObject::SetBoundaryMesh(vtkPolyData* mesh) {
	_boundaryMesh = vtkSmartPointer<vtkPolyData>::Take(mesh);
	this->SetBlock(1, _boundaryMesh);
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkPolyData> MGTMesh_MeshObject::GetBoundaryMesh() const {
	return _boundaryMesh;
}

//----------------------------------------------------------------------------
bool MGTMesh_MeshObject::IsEmpty() const {
	return (!_internalMesh
			   || (_internalMesh->GetNumberOfPoints() == 0
				   && _internalMesh->GetNumberOfCells() == 0))
		&& (!_boundaryMesh
			|| (_boundaryMesh->GetNumberOfPoints() == 0
				&& _boundaryMesh->GetNumberOfCells() == 0));
}