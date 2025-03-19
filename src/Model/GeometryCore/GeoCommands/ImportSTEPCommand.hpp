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

#ifndef IMPORTSTEPCOMMAND_HPP
#define IMPORTSTEPCOMMAND_HPP

#include "Command.hpp"
#include <string>
#include <vector>
#include <map>
#include <TopoDS_Shape.hxx>
#include "Geometry.hpp"
#include "GeoService.hpp"

class ImportSTEPCommand : public Command {

    public:

    ImportSTEPCommand(
        GeoService& aGeoService,
        const std::string& aFilePath
    );
    
    bool execute() override;
    bool undo() override;

    private:
    GeoService& _geoService;
    const std::string _filePath;
    std::map<std::string, TopoDS_Shape> _importedShapes;
};

#endif