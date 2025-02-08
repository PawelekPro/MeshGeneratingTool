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
* File      : Model.cpp
* Author    : Paweł Gilewicz, Krystian Fudali
* Date      : 30/01/2025
*/

#include "Model.hpp"
#include "ModelDocParser.hpp"

#include "MGTMeshUtils_ComputeError.hpp"
#include "MGTMesh_Algorithm.hpp"
#include "MGTMesh_Generator.hpp"

#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
Model::Model(std::string modelName)
	: _modelName(modelName)
	, _shapesMap(GeometryCore::PartsMap()) {
	// gmsh::model::add(_modelName);
	this->geometry = GeometryCore::Geometry();
	// this->mesh = MeshCore::Mesh();
};

//----------------------------------------------------------------------------
Model::~Model() {
	// gmsh::finalize();
}

//----------------------------------------------------------------------------
void Model::addShapesToModel(const GeometryCore::PartsMap& shapesMap) {
	for (const auto& it : shapesMap) {
		const std::string& key = it.first;
		const TopoDS_Shape& shape = it.second;

		if (_shapesMap.find(key) == _shapesMap.end()) {
			_shapesMap[key] = shape;
			spdlog::debug("Shape added to model: {}", key);
		}
		// std::vector<std::pair<int, int>> outDimTags;
		// gmsh::model::occ::importShapesNativePointer(shape_ptr, outDimTags);
	}
	// gmsh::model::occ::synchronize();
}

//----------------------------------------------------------------------------
void Model::importSTL(const std::string& filePath, QWidget* progressBar) {
	geometry.importSTL(filePath, progressBar);
	const GeometryCore::PartsMap& shapesMap = geometry.getShapesMap();
	addShapesToModel(shapesMap);
}

void Model::importSTEP(const std::string& filePath, QWidget* progressBar) {
	geometry.importSTEP(filePath, progressBar);
	const GeometryCore::PartsMap& shapesMap = geometry.getShapesMap();
	addShapesToModel(shapesMap);
}

//----------------------------------------------------------------------------
void Model::generateMesh(const MGTMesh_Algorithm* algorithm) {
	if (!algorithm)
		return;

	spdlog::debug(std::format("Mesh algorithm parameters - Engine: {}, type: {}, id: {}",
		algorithm->GetEngineLib(), algorithm->GetType(), algorithm->GetID()));

	for (const auto& it : _shapesMap) {
		spdlog::debug("Creating mesh generator for shape: {}", it.first);
		MGTMesh_Generator meshGenerator(it.second, *algorithm);
		int result = meshGenerator.Compute();
		if (result != MGTMeshUtils_ComputeErrorName::COMPERR_OK) {
			SPDLOG_ERROR("Error while generating mesh for shape: {}", it.first);
		}
	}
}