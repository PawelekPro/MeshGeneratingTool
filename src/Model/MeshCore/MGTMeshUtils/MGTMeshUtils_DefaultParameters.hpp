/*
* Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
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
* File      : MGTMesh_DefaultParameters.cpp
* Author    : Paweł Gilewicz
* Date      : 23/03/2024
*/

#ifndef MGTMeshUtils_DefaultParameters_HPP
#define MGTMeshUtils_DefaultParameters_HPP

class TopoDS_Shape;

class MGTMeshUtils_DefaultParameters {
public:
	static double GetDefaultMinSize(const TopoDS_Shape& geom, double maxSize);
};

#endif
