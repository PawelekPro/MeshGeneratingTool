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

#ifndef STEPPLUGINOPERATIONS_H
#define STEPPLUGINOPERATIONS_H

#include <filesystem>

#include <QWidget>

#include <vtkLogger.h>
#include <vtkProperty.h>

#include <BRep_Builder.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_DocumentTool.hxx>

#include "GeometryFunctions.h"
#include "ProgressBarPlugin.hpp"

namespace STEPPlugin {

/**
 * @brief  	The STEPFileReader provides class for importing STEP files.
 *
 * @details	Inherits from GeometryFunctions class.
 */
class STEPPluginOperations : public Geometry::GeometryFunctions {

public:
	/**
	 * Construct instance of STEPFileReader class.
	 *
	 */
	STEPPluginOperations() {};

	/**
	 * Destructor for instance of STEPFileReader class.
	 *
	 */
	~STEPPluginOperations() {};

	/**
	 * @brief  Load step/stp file and fill parts map container.
	 *
	 * @param  {std::string} fileName :  Path to step/stp file.
	 */
	void load(const std::string& fileName, QWidget* parent) override;

	/**
	 * @brief  Get container of vtkActor class instances.
	 *
	 * @return {ActorsMap}  : Container of vtkActor class instances.
	 */
	Geometry::ActorsMap getVTKActorsMap() override;

	/**
	 * @brief  Get container of vtkActor class instances representing edges.
	 *
	 * @return {ActorsMap}  : Container of vtkActor class instances.
	 */
	Geometry::ActorsMap getVTKEdgesMap();

	/**
	 * @brief  Get container of vtkActor class instances representing faces.
	 *
	 * @return {ActorsMap}  : Container of vtkActor class instances.
	 */
	Geometry::ActorsMap getVTKFacesMap();

	/**
	 * @brief  Get new unique label of edge object.
	 *
	 * @param  {std::string} prefix           : Prefix to be combined with unique id.
	 * @param  {Geometry::PartsMap} partsMap : Container of all loaded edges (occ shapes).
	 * @return {std::string}                  : Label containing unique index and given prefix.
	 */
	std::string getUniqueEdgeName(std::string prefix);

	/**
	 * @brief  Get new unique label of edge object.
	 *
	 * @param  {std::string} prefix           : Prefix to be combined with unique id.
	 * @param  {Geometry::PartsMap} partsMap : Container of all loaded edges (occ shapes).
	 * @return {std::string}                  : Label containing unique index and given prefix.
	 */
	std::string getUniqueFaceName(std::string prefix);

	/**
	 * @brief  Get map of objects representing edge shapes.
	 *
	 * @return {Geometry::PartsMap}  : Map of TopoDS_Shape shapes representing edges.
	 */
	Geometry::PartsMap getEdgesPartsMap();

	/**
	 * @brief  Get map of objects representing face shapes.
	 *
	 * @return {Geometry::PartsMap}  : Map of TopoDS_Shape shapes representing faces.
	 */
	Geometry::PartsMap getFacesPartsMap();

private:
	Geometry::PartsMap _edgesMap;
	Geometry::PartsMap _facesMap;
};

};

#endif