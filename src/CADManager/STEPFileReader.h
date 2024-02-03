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

#ifndef STEPFILEREADER_H
#define STEPFILEREADER_H

#include <filesystem>

#include <vtkLogger.h>
#include <vtkProperty.h>

#include <BRep_Builder.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_DocumentTool.hxx>

#include "CADFileReader.h"

namespace Importing {

/**
 * @brief  	The STEPFileReader provides class for importing STEP files.
 *
 * @details	Inherits from CADFileReader class.
 */
class STEPFileReader : public CADFileReader {

public:
	/**
	 * Construct instance of STEPFileReader class.
	 *
	 */
	STEPFileReader(ProgressBar* progressBar)
		: _progressBar(progressBar) { }

	/**
	 * Destructor for instance of STEPFileReader class.
	 *
	 */
	~STEPFileReader() {};

	/**
	 * @brief  Load step/stp file and fill parts map container.
	 *
	 * @param  {std::string} fileName :  Path to step/stp file.
	 */
	void load(const std::string& fileName) override;

	/**
	 * @brief  Get container of vtkActor class instances.
	 *
	 * @return {ActorsMap}  : Container of vtkActor class instances.
	 */
	ActorsMap getVTKActorsMap() override;

private:
	ProgressBar* _progressBar;
};

};

#endif