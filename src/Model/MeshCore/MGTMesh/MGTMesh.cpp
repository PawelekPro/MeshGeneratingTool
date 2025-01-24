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
* File      : MGTMesh.cpp
* Author    : Paweł Gilewicz
* Date      : 23/11/2024
*/
#include "MGTMesh.h"

vtkStandardNewMacro(MGTMesh);

//----------------------------------------------------------------------------
MGTMesh::MGTMesh()
	: _internalMesh(nullptr)
	, _boundaryMesh(nullptr) {
}

//----------------------------------------------------------------------------
MGTMesh::~MGTMesh() = default;

//----------------------------------------------------------------------------
void MGTMesh::SetInternalMesh(vtkSmartPointer<vtkUnstructuredGrid> mesh) {
	this->_internalMesh = mesh;
	this->SetBlock(0, this->_internalMesh);
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkUnstructuredGrid> MGTMesh::GetInternalMesh() const {
	return this->_internalMesh;
}

//----------------------------------------------------------------------------
void MGTMesh::SetBoundaryMesh(vtkSmartPointer<vtkPolyData> mesh) {
	this->_boundaryMesh = mesh;
	this->SetBlock(1, this->_boundaryMesh);
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkPolyData> MGTMesh::GetBoundaryMesh() const {
	return this->_boundaryMesh;
}
