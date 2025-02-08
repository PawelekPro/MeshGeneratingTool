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
#include "FileDialogUtils.hpp"
#include "ModelInterface.hpp"
#include "ImportGeometryCommand.hpp"
#include "CommandManager.hpp"

GeometryActionsHandler::GeometryActionsHandler(
    std::shared_ptr<ModelInterface> aModelInterface, 
    CommandManager* aCommandManager,
    GeometrySignalSender* aSignalSender,
    TreeStructure* aTreeStructure,
    QObject* aParent
    ) :
    QObject(aParent),
    _modelInterface(aModelInterface),
    _commandManager(aCommandManager),
    _geometrySignalSender(aSignalSender),
    _treeStructure(aTreeStructure){};
    
void GeometryActionsHandler::importSTEP(){
    QString filePath = FileDialogUtils::getFileSelection("Import STEP", FileDialogUtils::FilterSTEP);
    if (filePath.isEmpty()){
        qInfo("Import STEP cancelled");
        return;
    }
    ImportGeometryCommand* importCommand = new ImportGeometryCommand(
        _modelInterface,
        _geometrySignalSender, 
        _treeStructure,
        filePath
        );
    try {
        _commandManager->executeCommand(importCommand);
    } catch (const std::filesystem::filesystem_error& e) {
        qDebug() << "Error: File not found - " << e.what();
    } catch (const std::exception& e) {
        qDebug() << "Import STEP failed: " << e.what();
    } catch (...) {
        qDebug() << "An unknown error occurred while importing STEP file.";
    }

    // TODO: addShapes should send only the new shapes ids
    return;
}

void GeometryActionsHandler::importSTL(){
    QString filePath = FileDialogUtils::getFileSelection("Import STEP", FileDialogUtils::FilterSTEP);
if (filePath.isEmpty()){
        qInfo("Import STL cancelled");
        return;
    }
    _modelInterface->importSTL(filePath);
    return;
}