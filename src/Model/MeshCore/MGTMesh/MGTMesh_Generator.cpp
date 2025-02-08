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

//----------------------------------------------------------------------------
MGTMesh_Generator::MGTMesh_Generator(const TopoDS_Shape& shape, const MGTMesh_Algorithm& algorithm,
	std::shared_ptr<MGTMesh_MeshObject> meshObject)
	: _shape(&shape)
	, _algorithm(&algorithm)
	, _meshObject(meshObject) { }

//----------------------------------------------------------------------------
MGTMesh_Generator::~MGTMesh_Generator() { }

//----------------------------------------------------------------------------
std::shared_ptr<MGTMesh_MeshObject> MGTMesh_Generator::GetOutputMesh() { return _meshObject; }

//----------------------------------------------------------------------------
int MGTMesh_Generator::Compute() {
	if (_algorithm->GetEngineLib() == MGTMesh_Scheme::Engine::NETGEN) {
		std::unique_ptr<NetgenPlugin_Parameters> netgenAlg
			= std::make_unique<NetgenPlugin_Parameters>(_algorithm->GetID());

		NetgenPlugin_Mesher netgenMesher(_meshObject, *_shape, netgenAlg.get());
		return netgenMesher.ComputeMesh();
	}
	return MGTMeshUtils_ComputeErrorName::COMPERR_BAD_PARMETERS;
}
