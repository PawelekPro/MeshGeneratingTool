/*
 * Copyright (C) 2024 Krystian Fudali
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
 */

#ifndef MODEL_H
#define MODEL_H

#include "Geometry.h"
#include "Mesh.h"

#include <TopoDS_Shape.hxx>

#ifdef _WIN32
#include <gmsh.h_cwrap>
#endif

#ifdef linux
#include <gmsh.h>
#endif


#include <algorithm>
#include <vector>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkIdTypeArray.h>

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>


class Model {
public:
	std::string _modelName;
	GeometryCore::Geometry geometry;
	MeshCore::Mesh mesh;

	Model(std::string modelName);
	~Model();

	void updateParts();

	void meshParts();
	void updateMeshActor();

	void addSizing(std::vector<std::reference_wrapper<const TopoDS_Shape>> selectedShapes);

	vtkSmartPointer<vtkPolyData> createMeshVtkPolyData();
	vtkSmartPointer<vtkActor> meshActor;

private:
	enum class ElementType : int {
		TETRA = 4,
		TRIANGLE = 2,
		LINE = 1,
		POINT = 15,
	};
	vtkSmartPointer<vtkUnstructuredGrid> gridData;
	vtkSmartPointer<vtkPolyData> polyData;
};

#endif // MODEL_H
