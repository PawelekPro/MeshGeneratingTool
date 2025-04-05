/*
 * Copyright (C) 2024 Krystian Fudali
 *
 * This file is part of the Mesh Generating Tool.
 * (https://github.com/PawelekPro/MeshGeneratingTool)
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

#ifndef MODELACTIONSHANDLER_HPP
#define MODELACTIONSHANDLER_HPP

#include <QObject>
#include <memory>

class ModelInterface;
class CommandManager;
class RenderSignalSender;
class ProgressBar;
class TreeStructure;

class MeshActionsHandler;
class GeometryActionsHandler;

/**
 * Class that serves as a centrilzed access point to interaction with Model.
 * Each undo-able action should be added as a Command and be exectued with
 * CommandManager execute method. Model handler has sub-objects for handling
 * mesh and geometry actions separately.
 */
class ModelActionsHandler : public QObject {
	Q_OBJECT
public:
	ModelActionsHandler(std::shared_ptr<ModelInterface> aModelInterface,
		RenderSignalSender* aSignalSender, TreeStructure* aTreeStructure,
		ProgressBar* aProgressBar, QObject* aParent);
	/**
	 * @brief Action for that deletes old model and creates a new one.
	 */
	void createNewModel();

	/**
	 * @brief Calls CommandManager's undo method.
	 */
	void undo();

	GeometryActionsHandler* _geometryHandler;
	MeshActionsHandler* _meshHandler;
	RenderSignalSender* _renderSignalSender;

private:
	std::shared_ptr<ModelInterface> _modelInterface;

	CommandManager* _commandManager;

	TreeStructure* _treeStructure;
	ProgressBar* _progressBar;
};

#endif