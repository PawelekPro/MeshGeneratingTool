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
* File      : MGTMeshUtils_ComputeError.hpp
* Author    : Paweł Gilewicz
* Date      : 25/01/2025
*/
#ifndef MGTMESHUTILS_COMPUTEERROR_HPP
#define MGTMESHUTILS_COMPUTEERROR_HPP

enum MGTMeshUtils_ComputeErrorName {
	COMPERR_OK = -1,
	COMPERR_BAD_INPUT_MESH = -2, //!< wrong mesh on lower submesh
	COMPERR_STD_EXCEPTION = -3, //!< some std exception raised
	COMPERR_OCC_EXCEPTION = -4, //!< OCC exception raised
	COMPERR_SLM_EXCEPTION = -5, //!< SALOME exception raised
	COMPERR_EXCEPTION = -6, //!< other exception raised
	COMPERR_MEMORY_PB = -7, //!< std::bad_alloc exception
	COMPERR_ALGO_FAILED = -8, //!< algo failed for some reason
	COMPERR_BAD_SHAPE = -9, //!< bad geometry
	COMPERR_WARNING = -10, //!< algo reports error but sub-mesh is computed anyway
	COMPERR_CANCELED = -11, //!< compute canceled
	COMPERR_NO_MESH_ON_SHAPE = -12, //!< no mesh elements assigned to sub-shape
	COMPERR_BAD_PARMETERS = -13, //!< incorrect hypotheses parameters
	COMPERR_LAST_ALGO_ERROR = -100, //!< terminator of mesh computation errors
};

#endif