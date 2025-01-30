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
* File      : ModelActionsHandler.cpp
* Author    : Krystian Fudali, Paweł Gilewicz
* Date      : 28/01/2025
*/

#include "MeshActionsHandler.hpp"
#include "AddSizingCommand.hpp"
#include "CommandManager.hpp"
#include "ModelInterface.hpp"

// logging
#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
MeshActionsHandler::MeshActionsHandler(std::shared_ptr<ModelInterface> aModelInterface,
	CommandManager* aCommandManager, RenderSignalSender* aSignalSender,
	TreeStructure* aTreeStructure, ProgressBar* aProgressBar, QObject* aParent)
	: QObject(aParent)
	, _modelInterface(aModelInterface)
	, _commandManager(aCommandManager)
	, _signalSender(aSignalSender)
	, _treeStructure(aTreeStructure)
	, _progressBar(aProgressBar) { };

//----------------------------------------------------------------------------
void MeshActionsHandler::generate3DMesh() {
	SPDLOG_INFO("Generating volume mesh triggered.");
	_modelInterface->generateMesh();
	// TODO: send signal to update the renderer
}

//----------------------------------------------------------------------------
void MeshActionsHandler::generate2DMesh() {
	SPDLOG_INFO("Generating surface mesh triggered.");
	_modelInterface->generateMesh(true);
	// TODO: send signal to update the renderer
}

//----------------------------------------------------------------------------
void MeshActionsHandler::addSizingToShapes(const std::vector<int>& aShapesVec) {
	AddSizingCommand* sizingCommand
		= new AddSizingCommand(_signalSender->geometrySignals, _treeStructure);
	_commandManager->executeCommand(sizingCommand);
	return;
}

//----------------------------------------------------------------------------
void MeshActionsHandler::addSizingToSelectedShapes() {
	const std::vector<int> shapesIds = _signalSender->geometrySignals->getSelectedShapes();
	addSizingToShapes(shapesIds);
	return;
}