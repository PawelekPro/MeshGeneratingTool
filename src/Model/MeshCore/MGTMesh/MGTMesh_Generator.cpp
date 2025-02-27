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
* File      : MGTMesh_Generator.cpp
* Author    : Paweł Gilewicz
* Date      : 25/01/2025
*/

#include "MGTMesh_Generator.hpp"
#include "MGTMeshUtils_ComputeError.hpp"
#include "NetgenPlugin_Mesher.hpp"
#include "NetgenPlugin_Parameters.hpp"

#include <memory>

//----------------------------------------------------------------------------
MGTMesh_Generator::MGTMesh_Generator(
	const TopoDS_Shape& shape, const MGTMesh_Algorithm& algorithm,
	MGTMesh_MeshObject* meshObject)
	: _meshObject(meshObject)
	, _shape(&shape)
	, _algorithm(&algorithm) { }

//----------------------------------------------------------------------------
MGTMesh_Generator::~MGTMesh_Generator() = default;

//----------------------------------------------------------------------------
MGTMesh_MeshObject* MGTMesh_Generator::GetOutputMesh() const {
	return _meshObject;
}

//----------------------------------------------------------------------------
int MGTMesh_Generator::Compute() const {
	if (_algorithm->GetEngineLib() == MGTMesh_Scheme::Engine::NETGEN) {
		const auto netgenAlg = std::make_unique<NetgenPlugin_Parameters>(_algorithm->GetID());

		NetgenPlugin_Mesher netgenMesher(_meshObject, *_shape, netgenAlg.get());
		return netgenMesher.ComputeMesh();
	}
	return COMPERR_BAD_PARMETERS;
}
