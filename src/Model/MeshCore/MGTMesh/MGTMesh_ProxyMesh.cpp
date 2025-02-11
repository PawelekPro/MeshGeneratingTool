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

#include "MGTMesh_ProxyMesh.hpp"
#include "MGTMesh_MeshObject.hpp"

#include <vtkActor.h>
#include <vtkAppendFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkCompositeDataGeometryFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
MGTMesh_ProxyMesh::MGTMesh_ProxyMesh(MGTMesh_MeshObject* mgtMesh) {
	if (!mgtMesh) {
		_mgtMesh = mgtMesh;
		return;
	}

	// In case mgtMesh is a nullptr
	_mgtMesh = vtkSmartPointer<MGTMesh_MeshObject>::New();
}

//----------------------------------------------------------------------------
MGTMesh_ProxyMesh::~MGTMesh_ProxyMesh() { }

//----------------------------------------------------------------------------
MGTMesh_ProxyMesh::MGTMesh_ProxyMesh(
	std::unordered_map<int, std::shared_ptr<MGTMesh_MeshObject>> meshObjectsMap) {
	if (meshObjectsMap.empty())
		return;

	auto appendUnstructuredGrid = vtkSmartPointer<vtkAppendFilter>::New();
	auto appendPolyData = vtkSmartPointer<vtkAppendPolyData>::New();

	appendUnstructuredGrid->ReleaseDataFlagOn();
	appendPolyData->ReleaseDataFlagOn();

	for (auto& pair : meshObjectsMap) {
		int key = pair.first;
		std::shared_ptr<MGTMesh_MeshObject> meshObject = pair.second;
		spdlog::debug("Merging sub mesh with id: {}", key);

		if (!meshObject)
			continue;

		vtkSmartPointer<vtkUnstructuredGrid> internalMesh = meshObject->GetInternalMesh();
		if (internalMesh) {
			appendUnstructuredGrid->AddInputData(internalMesh);
		}

		vtkSmartPointer<vtkPolyData> boundaryMesh = meshObject->GetBoundaryMesh();
		if (boundaryMesh) {
			appendPolyData->AddInputData(boundaryMesh);
		}
	}

	appendUnstructuredGrid->Update();
	appendPolyData->Update();

	_mgtMesh = MGTMesh_MeshObject::New();
	_mgtMesh->SetInternalMesh(appendUnstructuredGrid->GetOutput());
	_mgtMesh->SetBoundaryMesh(appendPolyData->GetOutput());

	appendUnstructuredGrid->RemoveAllInputs();
	appendPolyData->RemoveAllInputs();
	appendUnstructuredGrid = nullptr;
	appendPolyData = nullptr;
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkActor> MGTMesh_ProxyMesh::GetProxyMeshActor() const {
	vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter
		= vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
	geometryFilter->SetInputData(_mgtMesh);
	geometryFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(geometryFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	actor->GetProperty()->SetEdgeVisibility(true);
	actor->GetProperty()->SetEdgeColor(1.0, 0.0, 0.0);

	return actor;
}