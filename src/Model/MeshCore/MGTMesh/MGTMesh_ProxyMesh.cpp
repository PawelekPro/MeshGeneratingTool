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
MGTMesh_ProxyMesh::~MGTMesh_ProxyMesh() = default;

//----------------------------------------------------------------------------
MGTMesh_ProxyMesh::MGTMesh_ProxyMesh(
	const std::unordered_map<int, vtkSmartPointer<MGTMesh_MeshObject>>&
		meshObjectsMap) {
	if (meshObjectsMap.empty())
		return;

	const auto appendUnstructuredGrid = vtkSmartPointer<vtkAppendFilter>::New();
	const auto appendPolyData = vtkSmartPointer<vtkAppendPolyData>::New();

	for (const auto& [fst, snd] : meshObjectsMap) {
		int key = fst;
		const vtkSmartPointer<MGTMesh_MeshObject> meshObject = snd;
		spdlog::debug("Merging sub mesh with id: {}", key);

		if (!meshObject)
			continue;

		if (vtkSmartPointer<vtkUnstructuredGrid> internalMesh
			= meshObject->GetInternalMesh()) {
			if (internalMesh->GetNumberOfCells() > 0) {
				appendUnstructuredGrid->AddInputData(internalMesh);
			} else {
				SPDLOG_WARN("Input data is not valid (internal mesh is empty)");
			}
		}

		if (vtkSmartPointer<vtkPolyData> boundaryMesh
			= meshObject->GetBoundaryMesh()) {
			if (boundaryMesh->GetNumberOfCells() > 0) {
				appendPolyData->AddInputData(boundaryMesh);
			} else {
				SPDLOG_WARN("Input data is not valid (boundary mesh is empty)");
			}
		}
	}

	appendUnstructuredGrid->Update();
	appendPolyData->Update();

	const auto internalMesh = appendUnstructuredGrid->GetOutput();
	const auto boundaryMesh = appendPolyData->GetOutput();

	_mgtMesh = MGTMesh_MeshObject::New();

	if (internalMesh->GetNumberOfCells() > 0) {
		_mgtMesh->SetInternalMesh(appendUnstructuredGrid->GetOutput());
	} else {
		SPDLOG_WARN("Output of filter for internal proxy mesh is empty");
	}

	if (boundaryMesh->GetNumberOfCells() > 0) {
		_mgtMesh->SetBoundaryMesh(appendPolyData->GetOutput());
	} else {
		SPDLOG_WARN("Output of filter for boundary proxy mesh is empty");
	}
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkActor> MGTMesh_ProxyMesh::GetProxyMeshActor() const {
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

	if (!_mgtMesh || _mgtMesh->IsEmpty()) {
		SPDLOG_WARN("MGTMesh_ProxyMesh::_mgtMesh is null or empty.");
		return actor;
	}

	const vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter
		= vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();

	geometryFilter->SetInputData(_mgtMesh);
	geometryFilter->Update();

	const vtkSmartPointer<vtkPolyDataMapper> mapper
		= vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(geometryFilter->GetOutputPort());

	actor->SetMapper(mapper);
	actor->GetProperty()->SetEdgeVisibility(true);
	actor->GetProperty()->SetEdgeColor(1.0, 0.0, 0.0);

	return actor;
}