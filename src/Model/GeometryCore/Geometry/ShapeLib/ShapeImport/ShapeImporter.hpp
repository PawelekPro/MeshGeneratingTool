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

#ifndef SHAPEIMPORTER_HPP
#define SHAPEIMPORTER_HPP

#include "GeoShape.hpp"
#include "ProgressIndicator.hpp"
#include <vector>
#include <string>
#include <fstream>

class ShapeImporter {
    
    public:
    virtual ~ShapeImporter() = default;

    std::vector<std::pair<TopoDS_Shape, ShapeAttr>> importFile(
        const std::string& aFilePath, 
        const ProgressIndicator& aProgressIndicator
    ) const;

    std::vector<std::pair<TopoDS_Shape, ShapeAttr>> 
        importFile(const std::string& aFilePath) const;
    
    virtual std::vector<std::pair<TopoDS_Shape, ShapeAttr>> import(
        std::istream& aFileStream, 
        const ProgressIndicator& aProgressIndicator
    ) const = 0;
};

#endif
