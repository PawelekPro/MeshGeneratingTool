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

#ifndef STLFILEREADER_H
#define STLFILEREADER_H

#include "../../UserInterface/GraphicalUtils/ProgressBar.h"
#include "../Geometry/GeometryFunctions.h"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <RWStl.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>

namespace STLPlugin {

/**
 * @brief  	The STLFileReader provides class for importing STL files.
 *
 * @details	Inherits from GeometryFunctions class.
 */
class STLFileReader : public Geometry::GeometryFunctions {
public:
	/**
	 * Constructor of STLFileReader class instance.
	 *
	 */
	STLFileReader() {};

	/**
	 * Destructor of STLFileReader class instance.
	 *
	 */
	~STLFileReader() {};

	/**
	 * @brief  Load STL file and fill parts map container.
	 *
	 * @param  {std::string} fileName : Path to the .stl file.
	 */
	void load(const std::string& fileName, QWidget* parent) override;

	/**
	 * @brief  Get container of vtkActor class instances.
	 *
	 * @return {ActorsMap}  : Container of vtkActor class instances.
	 */
	Geometry::ActorsMap getVTKActorsMap() override;
};

}

#endif