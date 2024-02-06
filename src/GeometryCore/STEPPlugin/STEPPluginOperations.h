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

#include <TColStd_HSequenceOfTransient.hxx>
#include <TCollection_AsciiString.hxx>

namespace STEPPlugin {
class STEPPluginOperations {
public:
	STEPPluginOperations() {};
	~STEPPluginOperations() {};

	/**
	 * @brief  Import STEP file.
	 *
	 * @param  {TCollection_AsciiString} fileName      : Name of the file to import.
	 * @return {Handle(TColStd_HSequenceOfTransient)}  : List of geometry objects
	 */
	Handle(TColStd_HSequenceOfTransient) importSTEP(const TCollection_AsciiString& fileName);
};
}

#endif