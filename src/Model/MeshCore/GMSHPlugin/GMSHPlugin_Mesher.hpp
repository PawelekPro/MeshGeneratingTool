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
* File      : GMSHPlugin_Mesher.hpp
* Author    : Paweł Gilewicz
* Date      : 23/02/2025
*/

#ifndef GMSHPLUGIN_MESHER_HPP
#define GMSHPLUGIN_MESHER_HPP

#include "GMSHPlugin_Defs.hpp"

class TopoDS_Shape;
class MGTMesh_MeshObject;
class GMSHPlugin_Parameters;

class GMSHPLUGIN_EXPORT GMSHPlugin_Mesher {
public:
	GMSHPlugin_Mesher(
		MGTMesh_MeshObject*, const TopoDS_Shape& shape, const GMSHPlugin_Parameters* algorithm);
	~GMSHPlugin_Mesher();
	int ComputeMesh();

private:
	void SetGmshOptions();

private:
	MGTMesh_MeshObject* _mesh;
	const TopoDS_Shape& _shape;
	const GMSHPlugin_Parameters* _algorithm;
};

#endif