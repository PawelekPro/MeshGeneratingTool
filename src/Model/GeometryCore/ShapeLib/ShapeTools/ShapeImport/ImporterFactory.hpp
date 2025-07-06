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

#ifndef IMPORTERFACTORY_HPP
#define IMPORTERFACTORY_HPP

#include <memory>
#include "ShapeImporter.hpp"
#include "STEPImporter.hpp"
#include "STLImporter.hpp"

enum class ImportFormat {
    STEP,
    STL
};

class ImporterFactory {
public:
    virtual ~ImporterFactory() = default;

    virtual std::shared_ptr<ShapeImporter> importer(
        ImportFormat format
    ) const = 0;
};

class FileImporterFactory : public ImporterFactory {
public:
    std::shared_ptr<ShapeImporter> importer(
        ImportFormat format
    ) const override {
        switch (format) {
            case ImportFormat::STEP:
                return std::make_unique<STEPImporter>();
            case ImportFormat::STL:
                return std::make_unique<STLImporter>();
            default:
                throw std::runtime_error("Unknown ImportFormat");
        }
    }
};

#endif
