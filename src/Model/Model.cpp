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
#include "ModelEvents.hpp"
#include "MGTMeshUtils_ComputeError.hpp"
#include "MGTMesh_Algorithm.hpp"
#include "MGTMesh_Generator.hpp"
#include "MGTMesh_MeshObject.hpp"
#include "MGTMesh_ProxyMesh.hpp"

#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
Model::Model(std::string modelName)
	: _modelName(modelName), 
	_proxyMesh(nullptr),
	geometry(subject, _commandStack){};

//----------------------------------------------------------------------------
Model::~Model() { _meshObjectsMap.clear(); }

//----------------------------------------------------------------------------
void Model::addShapesToModel(const GeometryCore::PartsMap& shapesMap) {
	for (const auto& it : shapesMap) {
		const std::string& key = it.first;
		const TopoDS_Shape& shape = it.second;
		GeometryCore::PartsMap geoShapesMap = geometry.getShapes();
		if (geoShapesMap.find(key) == geoShapesMap.end()) {
			geoShapesMap[key] = shape;
			spdlog::debug("Shape added to model: {}", key);
		}
		// std::vector<std::pair<int, int>> outDimTags;
		// gmsh::model::occ::importShapesNativePointer(shape_ptr, outDimTags);
	}
	// gmsh::model::occ::synchronize();
}

//----------------------------------------------------------------------------
void Model::importSTL(const std::string& filePath) {
	geometry.importSTL(filePath);
	const GeometryCore::PartsMap& shapesMap = geometry.getShapes();
	addShapesToModel(shapesMap);
}

void Model::importSTEP(const std::string& filePath) {
	geometry.importSTEP(filePath);
	const GeometryCore::PartsMap& shapesMap = geometry.getShapes();
	addShapesToModel(shapesMap);
}

//----------------------------------------------------------------------------
bool Model::generateMesh(const MGTMesh_Algorithm* algorithm) {
	if (!algorithm)
		return false;

	_meshObjectsMap.clear();

	spdlog::debug(std::format("Mesh algorithm parameters - Engine: {}, type: {}, id: {}",
		algorithm->GetEngineLib(), algorithm->GetType(), algorithm->GetID()));

	GeometryCore::PartsMap shapesMap = geometry.getShapes();
	for (const auto& [fst, snd] : shapesMap) {
		spdlog::debug("Creating mesh generator for shape: {}", fst);

		vtkSmartPointer<MGTMesh_MeshObject> meshObject = vtkSmartPointer<MGTMesh_MeshObject>::New();
		MGTMesh_Generator meshGenerator(snd, *algorithm, meshObject);
		if (const int result = meshGenerator.Compute();
			result != MGTMeshUtils_ComputeErrorName::COMPERR_OK) {
			SPDLOG_ERROR("Error while generating mesh for shape: {}", fst);
			return false;
		}

		_meshObjectsMap[algorithm->GetID()] = meshGenerator.GetOutputMesh();
	}
	_proxyMesh = std::make_shared<MGTMesh_ProxyMesh>(_meshObjectsMap);
	return true;
}

//----------------------------------------------------------------------------
MGTMesh_ProxyMesh* Model::getProxyMesh() const { return _proxyMesh.get(); }

void Model::addObserver(std::shared_ptr<EventObserver> aObserver){
    subject.attachObserver(aObserver);
}