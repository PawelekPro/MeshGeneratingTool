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
* File      : MGTMesh_Generator.hpp
* Author    : Paweł Gilewicz
* Date      : 25/01/2025
*/
#ifndef MGTMESH_GENERATOR_HPP
#define MGTMESH_GENERATOR_HPP

#include "MGTMesh_Algorithm.hpp"
#include "MGTMesh_MeshObject.hpp"

#include <memory>

#include <TopoDS_Shape.hxx>

class MGTMesh_Generator {
public:
	MGTMesh_Generator(const TopoDS_Shape&, const MGTMesh_Algorithm&,
		std::shared_ptr<MGTMesh_MeshObject> meshObject);
	~MGTMesh_Generator();

	int Compute();
	std::shared_ptr<MGTMesh_MeshObject> GetOutputMesh();

private:
	std::shared_ptr<MGTMesh_MeshObject> _meshObject;
	const TopoDS_Shape* _shape;
	const MGTMesh_Algorithm* _algorithm;
};

#endif