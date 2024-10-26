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
 * File      : MeshDS_MeshObject.h
 * Author    : Paweł Gilewicz
 * Date      : 26/10/2024
 */
#ifndef MeshDS_MESHOBJECT_H
#define MeshDS_MESHOBJECT_H

#include "MeshDS_Macro.h"

class MeshDS_EXPORT MeshDS_MeshObject {
public:
	virtual ~MeshDS_MeshObject() { }
};
#endif