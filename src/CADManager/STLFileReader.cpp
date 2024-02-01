/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 *
 * Created by Paweł Gilewicz on 01/02/2024.
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

#include "STLFileReader.h"

void Importing::STLFileReader::load(const std::string& fileName) {

	std::cout << "> importing STL file" << std::endl;

	TCollection_AsciiString aName((Standard_CString)fileName.data());
	OSD_Path aFile(aName);

	BRepBuilderAPI_Sewing shapeSewer;

	std::cout << " -> reading '" << fileName << "'" << std::endl;

	Handle(Poly_Triangulation) aSTLMesh = RWStl::ReadFile(aFile);

	Standard_Integer numberOfTriangles = aSTLMesh->NbTriangles();

	TopoDS_Vertex Vertex1, Vertex2, Vertex3;
	TopoDS_Shape shape;
	TopoDS_Face face;
	TopoDS_Wire wire;

	std::cout << " -> converting to faces" << std::endl;

	for (Standard_Integer i = 1; i <= numberOfTriangles; i++) {
		Poly_Triangle triangle = aSTLMesh->Triangle(i);

		Standard_Integer n1;
		Standard_Integer n2;
		Standard_Integer n3;

		triangle.Get(n1, n2, n3);

		gp_Pnt p1 = aSTLMesh->Node(n1);
		gp_Pnt p2 = aSTLMesh->Node(n2);
		gp_Pnt p3 = aSTLMesh->Node(n3);

		if (!p1.IsEqual(p2, 0.0) && !p1.IsEqual(p3, 0.0)) {
			Vertex1 = BRepBuilderAPI_MakeVertex(p1);
			Vertex2 = BRepBuilderAPI_MakeVertex(p2);
			Vertex3 = BRepBuilderAPI_MakeVertex(p3);

			wire = BRepBuilderAPI_MakePolygon(Vertex1, Vertex2, Vertex3, Standard_True);
			if (!wire.IsNull()) {
				face = BRepBuilderAPI_MakeFace(wire);
				if (!face.IsNull()) {
					shapeSewer.Add(face);
				}
			}
		}
	}

	std::cout << " -> sewing faces" << std::endl;

	shapeSewer.Perform();
	shape = shapeSewer.SewedShape();

	std::cout << " -> extracting shells" << std::endl;

	BRepBuilderAPI_MakeSolid solidmaker;
	TopTools_IndexedMapOfShape shellMap;
	TopExp::MapShapes(shape, TopAbs_SHELL, shellMap);

	unsigned int counter = 0;
	for (int ishell = 1; ishell <= shellMap.Extent(); ++ishell) {
		const TopoDS_Shell& shell = TopoDS::Shell(shellMap(ishell));
		solidmaker.Add(shell);
		counter++;
		std::string uniqueName = getUniqueObjectName("ShellPart");
		this->_partsMap[uniqueName] = shell;
	}

	std::cout << " -> shells found: " << counter << std::endl;

	std::cout << " -> converting to solid" << std::endl;

	TopoDS_Shape solid = solidmaker.Solid();

	std::cout << " -> done." << std::endl;
}

Importing::ActorsMap Importing::STLFileReader::getVTKActorsMap() {
	Importing::ActorsMap actorsMap {};

	for (const auto& it : this->_partsMap) {
		const auto& shape = it.second;

		vtkSmartPointer<vtkActor> actor = createVTKActor(shape);

		std::stringstream stringStream;
		stringStream << std::addressof(*actor.GetPointer());
		std::string actorKey = stringStream.str();

		actorsMap[actorKey] = actor;
	}

	return actorsMap;
}