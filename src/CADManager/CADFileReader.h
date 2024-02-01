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

#ifndef CADFILEMANAGER_H
#define CADFILEMANAGER_H

#include <iostream>
#include <map>
#include <string>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>

namespace Importing {
using namespace std::string_literals;
using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
using PartsMap = std::map<std::string, TopoDS_Shape>;

/**
 * @brief  The CADFileReader provides base class for any CAD file importer.
 *
 */
class CADFileReader {
protected:
	CADFileReader() {};
	virtual ~CADFileReader() {};
	/**
	 * @brief  Get new unique label of shape.
	 *
	 * @param  {std::string} prefix           : Prefix to be combined with unique id.
	 * @param  {Importing::PartsMap} partsMap : Container of all loaded parts (shapes).
	 * @return {std::string}                  : Label containing unique index and given prefix.
	 */
	std::string getUniqueObjectName(std::string prefix);

	// Container for loaded shapes.
	PartsMap _partsMap;

	// Data framework composed of labels and attributes.
	Handle(TDocStd_Document) _dataFrame;

public:
	/**
	 * @brief  Load CAD file.
	 *
	 * @param  {std::string} fileName : Path to file.
	 */
	virtual void load(const std::string& fileName) = 0;

	/**
	 * @brief  Get map of shapes converted into vtkActors.
	 *
	 */
	virtual ActorsMap getVTKActorsMap() = 0;

	/**
	 * @brief  Convert TopoDS_shape object into vtkActor.
	 *
	 * @param  {TopoDS_Shape} shape 		: Shape to be converted into vtkActor.
	 * @return {vtkSmartPointer<vtkActor>}  : Instance of vtkActor.
	 */
	vtkSmartPointer<vtkActor> createVTKActor(TopoDS_Shape shape);
};

}

#endif