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
* File      : NetgenPlugin_Netgen2VTK.cpp
* Author    : Paweł Gilewicz
* Date      : 23/11/2024
*/
#include "NetgenPlugin_Netgen2VTK.h"
#include "MGTMesh_MeshObject.hpp"

#include <vtkCellArray.h>
#include <vtkHexahedron.h>
#include <vtkLogger.h>
#include <vtkPoints.h>
#include <vtkQuad.h>
#include <vtkTetra.h>
#include <vtkTriangle.h>
#include <vtkUnsignedCharArray.h>

#include <meshing.hpp>

#include "spdlog/spdlog.h"

//----------------------------------------------------------------------------
NetgenPlugin_Netgen2VTK::NetgenPlugin_Netgen2VTK(const netgen::Mesh& netgenMesh)
	: _netgenMesh(netgenMesh) { }

//----------------------------------------------------------------------------
vtkSmartPointer<vtkPoints> NetgenPlugin_Netgen2VTK::PopulateMeshNodes() const {
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	const int nbN = static_cast<int>(_netgenMesh.GetNP());
	if (!nbN) {
		return points;
	}

	// Insert all node points
	for (int i = 1; i <= nbN; ++i) {
		const netgen::MeshPoint& mp = _netgenMesh.Point(i);
		const double coords[3] = { mp(0), mp(1), mp(2) };
		points->InsertNextPoint(coords[0], coords[1], coords[2]);
	}
	return points;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Netgen2VTK::ConvertToInternalMesh(
	MGTMesh_MeshObject* mesh) const {
	const int nbE = static_cast<int>(_netgenMesh.GetNE());
	if (!nbE) {
		SPDLOG_WARN("No volume elements found in the Netgen mesh.");
		return;
	}

	const vtkSmartPointer<vtkUnstructuredGrid> internalMesh
		= vtkUnstructuredGrid::New();

	const vtkSmartPointer<vtkPoints> points = this->PopulateMeshNodes();
	const vtkSmartPointer<vtkCellArray> cells
		= vtkSmartPointer<vtkCellArray>::New();
	const vtkSmartPointer<vtkUnsignedCharArray> cellTypes
		= vtkSmartPointer<vtkUnsignedCharArray>::New();

	for (int i = 1; i <= nbE; ++i) {
		const netgen::Element& elem = _netgenMesh.VolumeElement(i);
		const int elementType = elem.GetType();
		const auto vertices = elem.PNums();

		if (elementType == netgen::TET) { //! Tetrahedral element
			auto tetra = vtkSmartPointer<vtkTetra>::New();
			tetra->GetPointIds()->SetId(0, vertices[0] - 1);
			tetra->GetPointIds()->SetId(1, vertices[1] - 1);
			tetra->GetPointIds()->SetId(2, vertices[2] - 1);
			tetra->GetPointIds()->SetId(3, vertices[3] - 1);
			cells->InsertNextCell(tetra);
			cellTypes->InsertNextValue(VTK_TETRA);

		} else if (elementType == netgen::HEX) { //! Hexahedral element
			auto hex = vtkSmartPointer<vtkHexahedron>::New();
			for (int j = 0; j < 8; ++j) {
				hex->GetPointIds()->SetId(j, vertices[j] - 1);
			}
			cells->InsertNextCell(hex);
			cellTypes->InsertNextValue(VTK_HEXAHEDRON);

		} else {
			std::cerr << "Unsupported element type encountered: " << elementType
					  << std::endl;
		}
	}

	// Populate internal mesh data
	internalMesh->SetPoints(points);
	internalMesh->SetCells(cellTypes, cells);
	SPDLOG_INFO(
		"Internal mesh conversion completed: {} points, {} cells created.",
		points->GetNumberOfPoints(), cells->GetNumberOfCells());

	mesh->SetInternalMesh(internalMesh);
}

//----------------------------------------------------------------------------
void NetgenPlugin_Netgen2VTK::ConvertToBoundaryMesh(
	MGTMesh_MeshObject* mesh) const {
	const int nbSE = static_cast<int>(_netgenMesh.GetNSE());

	if (!nbSE) {
		SPDLOG_WARN("No surface elements found in the Netgen mesh.");
		return;
	}

	const vtkSmartPointer<vtkPolyData> boundaryMesh = vtkPolyData::New();
	const vtkSmartPointer<vtkPoints> points = this->PopulateMeshNodes();
	const vtkSmartPointer<vtkCellArray> polygons
		= vtkSmartPointer<vtkCellArray>::New();

	for (int i = 1; i <= nbSE; ++i) {
		const netgen::Element2d& elem = _netgenMesh.SurfaceElement(i);
		const int elementType = elem.GetType();

		const auto vertices = elem.PNums();

		// FIXME: Add support for higher-order elements
		// Note: Netgen indices are 1-based, VTK is 0-based
		if (elementType == netgen::TRIG) { //! Triangle element
			auto triangle = vtkSmartPointer<vtkTriangle>::New();
			triangle->GetPointIds()->SetId(0, vertices[0] - 1);
			triangle->GetPointIds()->SetId(1, vertices[1] - 1);
			triangle->GetPointIds()->SetId(2, vertices[2] - 1);
			polygons->InsertNextCell(triangle);

		} else if (elementType == netgen::QUAD) { //! Quad element
			auto quad = vtkSmartPointer<vtkQuad>::New();
			quad->GetPointIds()->SetId(0, vertices[0] - 1);
			quad->GetPointIds()->SetId(1, vertices[1] - 1);
			quad->GetPointIds()->SetId(2, vertices[2] - 1);
			quad->GetPointIds()->SetId(3, vertices[3] - 1);
			polygons->InsertNextCell(quad);

		} else {
			SPDLOG_ERROR(
				"Unsupported element type encountered: {}", elementType);
			continue;
		}
	}

	// Populate poly data
	boundaryMesh->SetPoints(points);
	boundaryMesh->SetPolys(polygons);
	SPDLOG_INFO(
		"Boundary mesh conversion completed: {} points, {} polygons created.",
		points->GetNumberOfPoints(), polygons->GetNumberOfCells());

	mesh->SetBoundaryMesh(boundaryMesh);
}
