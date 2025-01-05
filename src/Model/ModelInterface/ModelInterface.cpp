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

#include "ModelInterface.hpp"

ModelInterface::ModelInterface(ModelManager& aManager) :
                              _modelManager(aManager),
                              _modelDataView(aManager){};


int ModelInterface::importSTEP(const QString& aFilePath, QWidget* aWidget){
    Model& model = _modelManager.getModel();
    model.importSTEP(aFilePath.toStdString(), aWidget);
    return 0; //TODO return tags of imported shapes
}

int ModelInterface::importSTL(const QString& aFilePath, QWidget* aWidget){
    Model& model = _modelManager.getModel();
    model.importSTL(aFilePath.toStdString(), aWidget);
    return 0; //TODO return tags of imported shapes
}

void ModelInterface::createNewModel(const QString& aNewModelName){
    _modelManager.createNewModel(aNewModelName);
    return;
}

void ModelInterface::meshSurface(){
    Model& model = _modelManager.getModel();
    model.meshSurface();
};

void ModelInterface::meshVolume(){
    Model& model = _modelManager.getModel();
    model.meshVolume();
};
