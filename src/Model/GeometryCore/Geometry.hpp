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

#include "MessageBus.hpp"
#include "CommandStack.hpp"
#include "OcafShapeCore.hpp"
#include "GeoCommandsFactory.hpp"
#include "ShapeService.hpp"
#include "ShapeView.hpp"
#include "ShapeId.hpp"

class Geometry {

    public:
    Geometry(
        CommandStack& aCommandStack, 
        MessageBus& aMessageBus,
        std::shared_ptr<ShapeCore> aShapeCore
    );
    ~Geometry() = default;

    void importSTEP(const std::string& aFilePath);
    void importSTL(const std::string& aFilePath);
   
    void removeShape(const ShapeId& aId);
    void scaleShape(const ShapeId& aId, double aScaleFactor); 
   
    std::shared_ptr<ShapeView> shapeView() const {
        return _shapeView;
    } 
    
    private:
    CommandStack& _commandStack; 
    MessageBus& _eventBus;

    std::shared_ptr<ShapeCore> _shapeCore;
    std::shared_ptr<ShapeView> _shapeView;
    std::shared_ptr<ShapeService> _shapeService;
    GeoCommandsFactory _commandFactory;
};

#endif