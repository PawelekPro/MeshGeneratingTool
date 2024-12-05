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

GeometryActionsHandler::GeometryActionsHandler( QObject* aParent,
                                                ProgressBar* aProgressBar,
                                                std::shared_ptr<ModelInterface> aModelInterface) :
                                                QObject(aParent),
                                                _progressBar(aProgressBar),
                                                _modelInterface(aModelInterface){};


void GeometryActionsHandler::importSTEP(){
    QString filePath = FileDialogUtils::getFileSelection("Import STEP", FileDialogUtils::FilterSTEP);
    _modelInterface->importSTEP(filePath, _progressBar);
    return;
// TODO send signal to renderer to update the shapes
}

void GeometryActionsHandler::importSTL(){

    // _modelInterface->importSTL(aFilePath, _progressBar);
    return;
// TODO send signal to renderer to update the shapes
}