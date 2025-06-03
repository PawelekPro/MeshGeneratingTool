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

#ifndef AttrShapeMap_HPP 
#define AttrShapeMap_HPP 

#include <XCAFDoc_ShapeTool.hxx>
#include <unordered_map>
#include "ShapeMap.hpp"
#include "ShapeKey.hpp"

class AttrShapeMap : public ShapeMap {

    public:
    AttrShapeMap(Handle(XCAFDoc_ShapeTool) aShapeTool);
    ~AttrShapeMap() = default;
 
    bool containsId(const ShapeId& id) const override;
    bool containsShape(const TopoDS_Shape& id) const override;
    
    const TopoDS_Shape atId(const ShapeId& id) const override;
    const ShapeId atShape(const TopoDS_Shape& shape) const override;

    std::vector<ShapeIdPair> freeShapes() const override;
    std::vector<ShapeIdPair> subShapes(const ShapeId& id) const override; 

    protected:
    const ShapeId fromKey(const ShapeKey& aKey) const override; 

    private:
    Handle(XCAFDoc_ShapeTool) _shapeTool;
    TDF_Label findLabel(const ShapeKey& aKey) const;
    
};

#endif