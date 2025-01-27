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

#ifndef MESHACTIONSHANDELR_HPP
#define MESHACTIONSHANDELR_HPP

#include <QObject>
#include <memory>

#include "RenderSignalSender.hpp"

class ModelInterface;
class CommandManager;
class ProgressBar;
class TreeStructure;

/**
 * Class that serves as a centrilzed access point to interaction with Model' mesh module
 * Each undo-able action should be added as a Command and be exectued with CommandManager
 * execute method.
 */
class MeshActionsHandler : public QObject {
	Q_OBJECT

public:
	MeshActionsHandler(std::shared_ptr<ModelInterface> aModelInterface,
		CommandManager* aCommandManager,
		RenderSignalSender* aSignalSender,
		TreeStructure* aTreeStructure,
		ProgressBar* aProgressBar,
		QObject* aParent);

private:
	std::shared_ptr<ModelInterface> _modelInterface;

	CommandManager* _commandManager;
	RenderSignalSender* _signalSender;

	TreeStructure* _treeStructure;
	ProgressBar* _progressBar;

public slots:
	/**
	 * @brief Action that calls model's meshSurface. Model will generate a mesh based on settings
	 * stored in apps QDomDocument and create a vtkActor with mesh representation. The action clears
	 * the renderer and adds genereted mesh actor.
	 */
	void meshVolume();

	/**
	 * @brief Undoable action that creates fetches currently selected shapes ids
	 * and creates an ElementSizing TreeItem adding it to TreeStructure.
	 */
	void addSizingToSelectedShapes();

	/**
	 * @brief Undoable action that creates creates an ElementSizing tree item adding it to TreeStructure.
	 * @param aShapeIds - initial shapes ids added to created ElementSizing TreeITem.
	 */
	void addSizingToShapes(const std::vector<int>& aShapesIds);
};

#endif