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

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <string>
#include <vector>
#include <array>

#include "ModelSubject.hpp"
#include "CommandStack.hpp"
#include "OcafShapeCore.hpp"
#include "ShapeService.hpp"
#include "GeoCommandsFactory.hpp"

class Geometry {

    public:
    Geometry(CommandStack& aCommandStack, const ModelSubject& aModelSubject);
    ~Geometry() = default;

    void importSTEP(const std::string& aFilePath);
    
    private:
    CommandStack& _commandStack; 
    const ModelSubject& _subject;
   
    OcafShapeCore _shapeCore;
    
    ShapeService _shapeService;
    GeoCommandsFactory _commandFactory;
};

#endif