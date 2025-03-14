/*
 * Copyright (C) 2024 Krystian Fudali
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
 */

#include "GeometryActionsHandler.hpp"
#include "CommandManager.hpp"
#include "FileDialogUtils.hpp"
#include "ImportGeometryCommand.hpp"
#include "ModelInterface.hpp"

GeometryActionsHandler::GeometryActionsHandler(std::shared_ptr<ModelInterface> aModelInterface,
	CommandManager* aCommandManager, GeometrySignalSender* aSignalSender,
	TreeStructure* aTreeStructure, ProgressBar* aProgressBar, QObject* aParent)
	: QObject(aParent)
	, _modelInterface(aModelInterface)
	, _commandManager(aCommandManager)
	, _geometrySignalSender(aSignalSender)
	, _treeStructure(aTreeStructure)
	, _progressBar(aProgressBar) { };

void GeometryActionsHandler::importSTEP() {
	QString filePath
		= FileDialogUtils::getFileSelection("Import STEP", FileDialogUtils::FilterSTEP);

	if (filePath.isEmpty())
		return;

	ImportGeometryCommand* importCommand = new ImportGeometryCommand(
		_modelInterface, _progressBar, _geometrySignalSender, _treeStructure, filePath);

	_commandManager->executeCommand(importCommand);

	// TODO: addShapes should send only the new shapes ids
	return;
}

void GeometryActionsHandler::importSTL() {
	QString filePath
		= FileDialogUtils::getFileSelection("Import STEP", FileDialogUtils::FilterSTEP);

	if (filePath.isEmpty())
		return;

	_modelInterface->importSTL(filePath, _progressBar);
	return;
}