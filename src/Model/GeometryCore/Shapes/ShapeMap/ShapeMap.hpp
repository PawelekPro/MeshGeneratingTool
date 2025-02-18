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
 
#ifndef SHAPEMAP_HPP
#define SHAPEMAP_HPP

#include "ShapeId.hpp"
#include <TopoDS_Shape.hxx>

class ShapeMap {
    
    public:
    virtual ~ShapeMap() = default;

    virtual bool insertShape(const ShapeId& id, const TopoDS_Shape& shape) = 0;
    virtual bool removeShape(const ShapeId& id) = 0;
    
    virtual bool containsShape(const ShapeId& id) const = 0;

    virtual TopoDS_Shape getShape(const ShapeId& id) const = 0;
    virtual ShapeId getShapeId(const TopoDS_Shape& shape) const = 0;

    virtual std::vector<TopoDS_Shape> getAllTopLevelShapes() const = 0;
    virtual std::vector<TopoDS_Shape> getAllTopLevelIds() const = 0;
};

#endif