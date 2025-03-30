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

#ifndef GEOSHAPE_HPP
#define GEOSHAPE_HPP

#include <TopoDS_Shape.hxx>
#include "ShapeId.hpp"

struct ShapeColor{
    int r;
    int g;
    int b;
};

struct ShapeAttr{
    std::string name;
    ShapeColor color;
};

class GeoShape {

    public:
    GeoShape(
        const TopoDS_Shape& aShape,
        const ShapeId& aShapeId,
        const ShapeAttr& aAttr
    );
    
    const TopoDS_Shape shape() const;

    const std::string name() const;

    private:
    const TopoDS_Shape& _shape;
    ShapeAttr _attr;
    ShapeId _id;
};

#endif