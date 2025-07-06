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

#ifndef GEOCOMMANDSFACTORY_HPP
#define GEOCOMMANDSFACTORY_HPP

#include <memory>
#include <string>
#include "ImporterFactory.hpp"

class ImportShapesCommand;
class ShapeService;

class GeoCommandsFactory {

    public:    
    GeoCommandsFactory(
        std::shared_ptr<ShapeService> aShapeService,
        std::shared_ptr<ImporterFactory> aImporterFactory
    );

    std::unique_ptr<ImportShapesCommand> import(
        const std::string& aFilePath,
        ImportFormat aImportFormat
    ) const;

    private:
    std::shared_ptr<ShapeService> _shapeService;
    std::shared_ptr<ImporterFactory> _importerFactory;
};

#endif