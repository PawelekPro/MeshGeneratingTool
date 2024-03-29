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
 */

#include "GeometryFunctions.h"

vtkSmartPointer<vtkActor> GeometryCore::GeometryFunctions::createVTKActor(TopoDS_Shape shape) {
	IVtkOCC_Shape* vtkShapeAdapter = new IVtkOCC_Shape(shape);
	auto dataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
	dataSource->SetShape(vtkShapeAdapter);

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(dataSource->GetOutputPort());
	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	return actor;
}

std::string GeometryCore::GeometryFunctions::getUniquePartName(std::string prefix) {
	// Find already existing path that match prefix.
	std::vector<std::string> allNames;
	for (const auto& partsMapIt : this->_partsMap) {
		// String object is the first element of parts map.
		const std::string stringObj = partsMapIt.first;

		if (stringObj.find(prefix) != std::string::npos) {
			allNames.push_back(stringObj);
		}
	}

	int i = 0;
	std::string uniqueName;
	while (true) {
		std::stringstream stringStream;
		stringStream << prefix << std::setfill('0') << std::setw(3) << i;
		uniqueName = stringStream.str();
		auto res = std::find(std::begin(allNames), std::end(allNames), uniqueName);
		if (res == std::end(allNames)) {
			break;
		}
		i++;
	}
	return uniqueName;
}

GeometryCore::PartsMap GeometryCore::GeometryFunctions::getPartsMap(){
	return this->_partsMap;
}