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
 *
 * Mesh      : C++ implementation
 * File      : MeshMGT_MeshAttributes.h
 * Author    : Paweł Gilewicz
 * Date      : 27/10/2024
 */

#include "MeshMGT_Macro.h"

enum MeshDimension {
	MeshDim_0D = 0,
	MeshDim_1D,
	MeshDim_2D,
	MeshDim_3D
};

class MeshMGT_EXPORT MeshMGT_MeshAttributes {
public:
	enum Status // in the order of severity
	{
		ATT_OK = 0,
		ATT_MISSING, //! algo misses a attributes
		ATT_BAD_PARAMETER, //! Attributes has a bad parameter value
		ATT_UNKNOWN_FATAL, //! all statuses below should be considered as fatal
						   //!      for Add/RemoveATTothesis operations
		ATT_INCOMPATIBLE, //! Atrtibutes does not fit algorithm
		ATT_ALREADY_EXIST, //! several applicable Attributes of same priority assigned
		ATT_BAD_DIM, //! bad dimension
		ATT_BAD_SUBSHAPE, //! shape is neither the main one, nor its sub-shape, nor a group
		ATT_BAD_GEOMETRY, //! shape geometry mismatches algorithm's expectation
		ATT_NEED_SHAPE, //! algorithm can work on shape only
	};

	MeshMGT_MeshAttributes();
	virtual ~MeshMGT_MeshAttributes();
};
