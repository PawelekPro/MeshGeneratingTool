// /*
//  * Copyright (C) 2024 Paweł Gilewicz
//  *
//  * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
//  *
//  * This program is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  */

// #ifndef STEPPLUGINIMPORT_H
// #define STEPPLUGINIMPORT_H

// #include "STEPPluginOperations.h"

// #include <map>
// #include <string>

// #include <vtkActor.h>
// #include <vtkSmartPointer.h>

// #include <TopoDS_Shape.hxx>

// class STEPGeometryPlugin {
// public:
// 	STEPGeometryPlugin();
// 	~STEPGeometryPlugin() = default;

// 	/**
// 	 * @brief  Load step/stp file and fill shapes map container.
// 	 *
// 	 * @param  {std::string} fileName :  Path to step/stp file.
// 	 */
// 	void load(const std::string& fileName, QWidget* parent);

// 	/**
// 	 * @brief  Get merged map containing all VTK actors representing loaded geometry.
// 	 *
// 	 * @return {GeometryCore::ActorsMap}  : Map containing all actors for created shapes
// 	 */
// 	GeometryCore::ActorsMap getVTKActorsMap();

// 	// Containers for vtkActor and occ shapes
// 	GeometryCore::PartsMap edgesPartsMap;
// 	GeometryCore::PartsMap facesPartsMap;
// 	GeometryCore::ActorsMap edgesActorsMap;
// 	GeometryCore::ActorsMap facesActorsMap;
	
// 	// Attribute for handling operations on step file
// 	STEPPlugin::STEPPluginOperations stepOperations;
// private:

// };

// #endif