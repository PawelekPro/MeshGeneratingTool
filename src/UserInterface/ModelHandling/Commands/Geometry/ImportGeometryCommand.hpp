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

#ifndef IMPORTGEOMETRYCOMMAND_HPP
#define IMPORTGEOMETRYCOMMAND_HPP


#include "Command.hpp"

#include <memory>
#include <QString>

class ProgressBar;
class TreeStructure;
class TreeItem;
class ModelInterface;
class GeometrySignalSender;

/**
 * Command for importing geometry file into the model. Calls import geometry from modelInterface and creates
 * ImportSTEP item in TreeStrucutre.
 */
class ImportGeometryCommand : public Command{

    public: 
    ImportGeometryCommand(std::shared_ptr<ModelInterface> aModelInterface,
                          ProgressBar* aProgressBar,
                          GeometrySignalSender* aSignalSender,
                          TreeStructure* aTreeStructure,
                          const QString& aFilePath);

    /**
    * @brief Command's execute method - calls importSTEP from modelInterface creates an ImportSTEP TreeItem
    * in TreeStrucutre and keeps its pointer.
    */
    void execute() override;
    
    /**
    * @brief Command's undo method - removes imported shapes from model and removes impotSTEP TreeItem.
    */
    void undo() override;

    private:
    ProgressBar* _progressBar;
    GeometrySignalSender* _signalSender;
    TreeStructure* _treeStructure;
    std::shared_ptr<ModelInterface> _modelInterface;

    const QString _importedFilePath;
    std::vector<int> _importedShapesTags;
    TreeItem* _treeItem;
};

#endif
