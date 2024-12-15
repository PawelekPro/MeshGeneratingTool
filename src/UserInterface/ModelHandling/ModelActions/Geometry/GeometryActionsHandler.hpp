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

class GeometryActionsHandler : public QObject{
    Q_OBJECT

    public:

    GeometryActionsHandler(std::shared_ptr<ModelInterface> aModelInterface, 
                           CommandManager* aCommandManager,
                           GeometrySignalSender* aGeometrySignalSender,
                           TreeStructure* aTreeStructure,
                           ProgressBar* aProgressBar, 
                           QObject* aParent);

    private:

    std::shared_ptr<ModelInterface> _modelInterface;
    
    CommandManager* _commandManager;
    GeometrySignalSender* _geometrySignalSender;

    TreeStructure* _treeStructure;
    ProgressBar* _progressBar;

    public slots:
    void importSTEP();
    void importSTL();

};
#endif