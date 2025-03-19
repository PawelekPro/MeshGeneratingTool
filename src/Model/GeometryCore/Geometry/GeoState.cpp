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

#include "GeoState.hpp"

void GeoState::addShapes(const std::map<std::string, TopoDS_Shape>& aShapes){
    for (auto shapePair : aShapes){
        _shapes.insert(shapePair);
    }
}

std::map<std::string, TopoDS_Shape> GeoState::shapes() const {
    return _shapes;
}

void GeoState::removeShapes(const std::vector<TopoDS_Shape>& aShapes){
    // This will be reworked, i am just trying to push command pattern out.
    for (auto shape : aShapes){
        for (auto shapePair : _shapes){
            if (shape == shapePair.second){
                _shapes.erase(shapePair.first);
            }
        }
    }
} 