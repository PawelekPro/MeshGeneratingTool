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

#include "ModelActionsHandler.hpp"
#include "ModelInterface.hpp"
#include "CommandManager.hpp"

#include "GeometryActionsHandler.hpp"
#include "MeshActionsHandler.hpp"
#include "RenderSignalSender.hpp"

ModelActionsHandler::ModelActionsHandler(
    std::shared_ptr<ModelInterface> aModelInterface, 
    RenderSignalSender* aSignalSender,
    TreeStructure* aTreeStructure,
    ProgressBar* aProgressBar, 
    QObject* aParent
    ) :
    QObject(aParent),
    _modelInterface(aModelInterface),
    _commandManager(new CommandManager(this)),
    _renderSignalSender(aSignalSender),
    _treeStructure(aTreeStructure),
    _progressBar(aProgressBar)
    {
        _geometryHandler = new GeometryActionsHandler(aModelInterface,
        _commandManager,
        _renderSignalSender->geometrySignals,
        _treeStructure,
        aProgressBar,
        aParent);

        _meshHandler = new MeshActionsHandler(aModelInterface,
        _commandManager,
        _renderSignalSender->meshSignals,
        _treeStructure,
        aProgressBar,
        aParent);
    };
void ModelActionsHandler::createNewModel(){
    //TODO: Handle new model name
    //TODO: send signals that will clear renderer, treeStructure etc.
    _modelInterface->createNewModel("NewModel");
}