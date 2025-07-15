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

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>
#include <TopoDS_Shape.hxx>
#include <TopLoc_Location.hxx>

#include "ShapeId.hpp"
#include "Color.hpp"

class Shape {
    public:
    virtual ~Shape() = default;

    virtual ShapeId id() const = 0;

    virtual bool isAssembly() const = 0;
    virtual TopoDS_Shape shape() const = 0;
    virtual TopLoc_Location location() const = 0;
    
    virtual std::string name() const = 0;
    virtual ColorRGBA color() const = 0;
};

#endif