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
	COMPERR_OK = 0,
	COMPERR_BAD_INPUT_MESH = -1, //!< wrong mesh on lower submesh
	COMPERR_STD_EXCEPTION = -2, //!< some std exception raised
	COMPERR_OCC_EXCEPTION = -3, //!< OCC exception raised
	COMPERR_MGT_EXCEPTION = -4, //!< MGT exception raised
	COMPERR_EXCEPTION = -5, //!< other exception raised
	COMPERR_MEMORY_PB = -6, //!< std::bad_alloc exception
	COMPERR_ALGO_FAILED = -7, //!< algo failed for some reason
	COMPERR_BAD_SHAPE = -8, //!< bad geometry
	COMPERR_WARNING = -9, //!< algo reports error but sub-mesh is computed anyway
	COMPERR_CANCELED = -10, //!< compute canceled
	COMPERR_NO_MESH_ON_SHAPE = -11, //!< no mesh elements assigned to sub-shape
	COMPERR_BAD_PARMETERS = -12, //!< incorrect hypotheses parameters
	COMPERR_LAST_ALGO_ERROR = -100, //!< terminator of mesh computation errors
};

#endif