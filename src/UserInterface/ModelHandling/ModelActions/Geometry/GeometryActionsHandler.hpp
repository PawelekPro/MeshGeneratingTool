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

#ifndef GEOMETRYACTIONSHANDELR_HPP
#define GEOMETRYACTIONSHANDELR_HPP

#include <memory>
#include <QObject>

#include "GeometrySignalSender.hpp"

class ModelInterface;
class CommandManager;
class ProgressBar;
class TreeStructure;

/**
 * Class that serves as a centrilzed access point to interaction with Model's gemetry module
 * Each undo-able action should be added as a Command and be exectued with CommandManager 
 * execute method.
 */
class GeometryActionsHandler : public QObject{
    Q_OBJECT

    public:

    GeometryActionsHandler(std::shared_ptr<ModelInterface> aModelInterface, 
                           CommandManager* aCommandManager,
                           GeometrySignalSender* aGeometrySignalSender,
                           TreeStructure* aTreeStructure, 
                           QObject* aParent);

    private:

    std::shared_ptr<ModelInterface> _modelInterface;
    
    CommandManager* _commandManager;
    GeometrySignalSender* _geometrySignalSender;

    TreeStructure* _treeStructure;
    public slots:

    /**
     * @brief Undoable action that opens fileDialog for user to selecd STEP file to be imported.
     * STEP geometry is split into shapes and added to model.geometry. Execution clears the renderer
     * and displays newly imported shapes. ImportSTEP TreeItem is created and added to the TreeStructure.
     */
    void importSTEP();

    /**
     * @brief Undoable action that opens fileDialog for user to selecd STL file to be imported.
     * STL geometry is split into shapes and added to model.geometry. Execution clears the renderer
     * and displays newly imported shapes. ImportSTEP TreeItem is created and added to the TreeStructure.
     */
    void importSTL();

};

#endif