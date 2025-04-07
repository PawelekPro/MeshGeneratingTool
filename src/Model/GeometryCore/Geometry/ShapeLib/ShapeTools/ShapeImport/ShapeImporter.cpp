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

#include "ShapeImporter.hpp"
#include <stdexcept>

std::vector<std::pair<TopoDS_Shape, ShapeAttr>> ShapeImporter::importFile(
    const std::string& aFilePath, 
    const ProgressIndicator& aProgressIndicator
) const {
    std::ifstream file(aFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + aFilePath);
    }
    return import(file, aProgressIndicator);
}

std::vector<std::pair<TopoDS_Shape, ShapeAttr>> ShapeImporter::importFile(
    const std::string& aFilePath
) const {
    IdleProgressIndicator progressIndicator;
    return importFile(aFilePath, progressIndicator);
}