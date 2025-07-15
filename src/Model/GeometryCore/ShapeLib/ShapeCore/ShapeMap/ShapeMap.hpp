/*
 * Copyright (C) 2024 Krystian Fudali
 *
 * This file is Shape of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A ShapeICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef SHAPEMAP_HPP
#define SHAPEMAP_HPP

#include "ShapeId.hpp"
#include "Shape.hpp"
#include "ShapeIdFactory.hpp"
#include <TopoDS_Shape.hxx>
#include <memory>

class ShapeCore;
class ShapeMap {
    
    public:
    virtual ~ShapeMap() = default;
 
    virtual bool containsId(const ShapeId& id) const = 0;
    virtual bool containsTShape(TopoDS_Shape id) const = 0;
    virtual bool containsShape(std::shared_ptr<Shape> shape) const = 0;
    
    virtual std::shared_ptr<Shape> atId(const ShapeId& id) const = 0;
    virtual std::shared_ptr<Shape> shapeFromTopo(
        TopoDS_Shape shape
    ) const = 0;

    virtual std::vector<std::shared_ptr<Shape>> freeShapes() const = 0;
    virtual std::vector<std::shared_ptr<Shape>> assemblyShapes(
        ShapeId&
    ) const = 0;

    virtual std::shared_ptr<Shape> parentAssembly(ShapeId&) const = 0;

    protected:
    friend class ShapeCore;

};

#endif