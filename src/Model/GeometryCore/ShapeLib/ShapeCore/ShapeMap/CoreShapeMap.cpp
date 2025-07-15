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

#include "CoreShapeMap.hpp"
#include "LabelKeyTool.hpp"
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>

CoreShapeMap::CoreShapeMap(
    Handle(XCAFDoc_ShapeTool) aShapeTool
) : ShapeMap(), 
    _shapeTool(aShapeTool){}

bool CoreShapeMap::containsId(const ShapeId& id) const {
    std::shared_ptr<ShapeKey> key = ShapeIdFactory::getKey(id);
    TDF_Label shapeLabel = LabelKeyTool::labelFromKey(
        _shapeTool->BaseLabel().Root(), key
    );
    TopoDS_Shape shape = _shapeTool->GetShape(shapeLabel);
    return !shape.IsNull();
}

bool CoreShapeMap::containsTShape(TopoDS_Shape shape) const {
    TDF_Label label;
    return _shapeTool->Search(shape, label);
}

bool CoreShapeMap::containsShape(std::shared_ptr<Shape> aShape) const {
    TDF_Label label;
    return _shapeTool->Search(aShape->shape(), label);
}

std::shared_ptr<Shape> CoreShapeMap::atId(const ShapeId& id) const {
    std::shared_ptr<ShapeKey> key = ShapeIdFactory::getKey(id);
    TDF_Label shapeLabel = LabelKeyTool::labelFromKey(
        _shapeTool->BaseLabel().Root(), key
    );
    TopoDS_Shape shape = _shapeTool->GetShape(shapeLabel);
    return shapeFromTopo(shape);
}

std::vector<std::shared_ptr<Shape>> CoreShapeMap::freeShapes() const {
    TDF_LabelSequence freeShapeLabels;
    _shapeTool->GetFreeShapes(freeShapeLabels);
    std::vector<std::shared_ptr<Shape>> shapes(freeShapeLabels.Size());
    for (auto label : freeShapeLabels){
        std::shared_ptr<ShapeKey> key = LabelKeyTool::keyFromLabel(label);
        auto id = ShapeIdFactory::create(key);
        shapes.push_back(atId(id));
    }
    return shapes;
}