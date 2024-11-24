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

void ModelInterface::importSTEP(const QString& filePath, QWidget* progressBar){
    std::string stdFilePath = filePath.toStdString();
    try {
        Model& model = _modelManager.getModel();
        model.importSTEP(stdFilePath, progressBar);
    } catch (const std::runtime_error& e) {
        qDebug() << "Error: " << e.what();
    }
}

void ModelInterface::meshSurface() {
    try {
        Model& model = _modelManager.getModel();
        model.meshSurface();
        qDebug() << "Surface mesh generated successfully.";
    } catch (const std::runtime_error& e) {
        qDebug() << "Error during surface meshing: " << e.what();
    }
}

void ModelInterface::meshVolume() {
    try {
        Model& model = _modelManager.getModel();
        model.meshVolume();
        qDebug() << "Volume mesh generated successfully.";
    } catch (const std::runtime_error& e) {
        qDebug() << "Error during volume meshing: " << e.what();
    }
}

