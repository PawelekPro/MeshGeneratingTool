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
* File      : NetgenPlugin_Netgen2VTK.cpp
* Author    : Paweł Gilewicz
* Date      : 23/11/2024
*/
#include "NetgenPlugin_Netgen2VTK.h"

#include <vtkCellArray.h>
#include <vtkHexahedron.h>
#include <vtkLogger.h>
#include <vtkPoints.h>
#include <vtkQuad.h>
#include <vtkTetra.h>
#include <vtkTriangle.h>
#include <vtkUnsignedCharArray.h>

#include <meshing.hpp>

//----------------------------------------------------------------------------
NetgenPlugin_Netgen2VTK::NetgenPlugin_Netgen2VTK(
	const netgen::Mesh& netgenMesh)
	: _netgenMesh(netgenMesh)
	, _internalMesh(vtkSmartPointer<vtkUnstructuredGrid>::New())
	, _boundaryMesh(vtkSmartPointer<vtkPolyData>::New()) {
	this->ConvertToBoundaryMesh();
	this->ConvertToInternalMesh();
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkPoints> NetgenPlugin_Netgen2VTK::PopulateMeshNodes() {
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	const int nbN = (int)_netgenMesh.GetNP();
	if (!nbN) {
		return points;
	}

	// Insert all node points
	for (int i = 1; i <= nbN; ++i) {
		const netgen::MeshPoint& mp = _netgenMesh.Point(i);
		double coords[3] = { mp(0), mp(1), mp(2) };
		points->InsertNextPoint(coords[0], coords[1], coords[2]);
	}
	return points;
}

//----------------------------------------------------------------------------
void NetgenPlugin_Netgen2VTK::ConvertToInternalMesh() {
	const int nbE = (int)_netgenMesh.GetNE();
	if (!nbE) {
		vtkLogF(WARNING, "No volume elements found in the Netgen mesh.");
		return;
	}

	auto points = this->PopulateMeshNodes();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	auto cellTypes = vtkSmartPointer<vtkUnsignedCharArray>::New();

	for (int i = 1; i <= nbE; ++i) {
		const netgen::Element& elem = _netgenMesh.VolumeElement(i);
		int elementType = elem.GetType();
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
			std::cerr << "Unsupported element type encountered: " << elementType << std::endl;
		}
	}

	// Populate internal mesh data
	_internalMesh->SetPoints(points);
	_internalMesh->SetCells(cellTypes, cells);
	vtkLogF(INFO, "Conversion completed: %d points, %d cells created.",
		points->GetNumberOfPoints(), cells->GetNumberOfCells());
}

//----------------------------------------------------------------------------
void NetgenPlugin_Netgen2VTK::ConvertToBoundaryMesh() {
	const int nbSE = (int)_netgenMesh.GetNSE();

	if (!nbSE) {
		vtkLogF(WARNING, "No surface elements found in the Netgen mesh.");
		return;
	}

	auto points = this->PopulateMeshNodes();
	auto polygons = vtkSmartPointer<vtkCellArray>::New();

	for (int i = 1; i <= nbSE; ++i) {
		const netgen::Element2d& elem = _netgenMesh.SurfaceElement(i);
		int elementType = elem.GetType();

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
			vtkLogF(ERROR, "Unsupported element type encountered: %d", elementType);
			continue;
		}
	}

	// Populate poly data
	_boundaryMesh->SetPoints(points);
	_boundaryMesh->SetPolys(polygons);
	vtkLogF(INFO, "Conversion completed: %d points, %d polygons created.",
		points->GetNumberOfPoints(), polygons->GetNumberOfCells());
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkUnstructuredGrid> NetgenPlugin_Netgen2VTK::GetInternalMesh() {
	return _internalMesh;
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkPolyData> NetgenPlugin_Netgen2VTK::GetBoundaryMesh() {
	return _boundaryMesh;
}