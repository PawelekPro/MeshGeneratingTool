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

#ifndef CORESHAPEMAP_HPP 
#define CORESHAPEMAP_HPP 

#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_Label.hxx>
#include <Standard_Handle.hxx>
#include <unordered_map>
#include "ShapeMap.hpp"
#include "ShapeKey.hpp"

class CoreShapeMap : public ShapeMap {

    public:
    CoreShapeMap(Handle(XCAFDoc_ShapeTool) aShapeTool);
    ~CoreShapeMap() = default;
 
    bool containsId(const ShapeId& id) const override; 
    bool containsTShape(TopoDS_Shape id) const override; 
    bool containsShape(std::shared_ptr<Shape> shape) const override;
    
    std::shared_ptr<Shape> atId(const ShapeId& id) const override;
    std::shared_ptr<Shape> shapeFromTopo(
        TopoDS_Shape shape
    ) const override {
        std::shared_ptr<Shape> ptr;
        return ptr;
    }

    std::vector<std::shared_ptr<Shape>> freeShapes() const override;
    std::vector<std::shared_ptr<Shape>> assemblyShapes(
        ShapeId&
    ) const override {return std::vector<std::shared_ptr<Shape>>();};

    std::shared_ptr<Shape> parentAssembly(ShapeId&) const override{
        std::shared_ptr<Shape> ptr;
        return ptr;
    }

    private:
    Handle(XCAFDoc_ShapeTool) _shapeTool;
};

#endif