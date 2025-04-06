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
* File      : ModelInterface.cpp
* Author    : Krystian Fudali, Paweł Gilewicz
* Date      : 28/01/2025
*/

#include "ModelInterface.hpp"
#include "ModelDocParser.hpp"

#include "MGTMesh_Algorithm.hpp"

#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
ModelInterface::ModelInterface(ModelManager& aModelManager)
	: _modelManager(aModelManager)
	, _modelDataView(aModelManager) { };

//----------------------------------------------------------------------------
int ModelInterface::importSTEP(
	const QString& aFilePath, QWidget* progressBar) const {
	Model& model = _modelManager.getModel();
	model.importSTEP(aFilePath.toStdString(), progressBar);
	return 0; // TODO return tags of imported shapes
}

//----------------------------------------------------------------------------
int ModelInterface::importSTL(
	const QString& aFilePath, QWidget* progressBar) const {
	Model& model = _modelManager.getModel();
	model.importSTL(aFilePath.toStdString(), progressBar);
	return 0; // TODO return tags of imported shapes
}

//----------------------------------------------------------------------------
void ModelInterface::createNewModel(const QString& aNewModelName) const {
	_modelManager.createNewModel(aNewModelName);
}

//----------------------------------------------------------------------------
bool ModelInterface::generateMesh(bool surfaceMesh,
	const std::function<void(int)>& progressCallback,
	const std::function<void(const std::string&)>& statusCallback) const {
	spdlog::debug(
		std::format("Mesh generation process started with surfaceMesh arg: {}",
			surfaceMesh));

	Model& model = _modelManager.getModel();
	const ModelDocParser modelDocument(model);
	const std::unique_ptr<MGTMesh_Algorithm> algorithm
		= modelDocument.generateMeshAlgorithm(surfaceMesh);

	return model.generateMesh(
		algorithm.get(), progressCallback, statusCallback);
}
//----------------------------------------------------------------------------
void ModelInterface::CancelMeshGeneration() const {
	Model& model = _modelManager.getModel();
	model.CancelMeshGeneration();
}