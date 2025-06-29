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
#include <TopExp_Explorer.hxx>

using ShapeIdPair = ShapeMap::ShapeIdPair;

CoreShapeMap::CoreShapeMap(
    Handle(XCAFDoc_ShapeTool) aShapeTool
) : ShapeMap(), 
    _shapeTool(aShapeTool), 
    _labelKeyTool(std::make_unique<LabelKeyTool>(aShapeTool)){}

bool CoreShapeMap::containsId(const ShapeId& id) const {
    std::shared_ptr<ShapeKey> key = ShapeIdFactory::getKey(id);
    TDF_Label shapeLabel = _labelKeyTool->labelFromKey(key);
    TopoDS_Shape shape = _shapeTool->GetShape(shapeLabel);
    return !shape.IsNull();
}

bool CoreShapeMap::containsShape(const TopoDS_Shape& shape) const {
    TDF_Label label;
    return _shapeTool->Search(shape, label);
}

const TopoDS_Shape CoreShapeMap::atId(const ShapeId& id) const {
    std::shared_ptr<ShapeKey> key = ShapeIdFactory::getKey(id);
    TDF_Label shapeLabel = _labelKeyTool->labelFromKey(key);
    TopoDS_Shape shape = _shapeTool->GetShape(shapeLabel);
    return shape;
}

const ShapeId CoreShapeMap::atShape(const TopoDS_Shape& shape) const {
    TDF_Label label;
    _shapeTool->Search(shape, label);
    if (label.IsNull()) {
        return ShapeId::invalidId();
    }

    auto key = _labelKeyTool->keyFromLabel(label);
    ShapeId id = ShapeIdFactory::create(key);
    return id;
}

std::vector<ShapeIdPair> CoreShapeMap::freeShapes() const {
    TDF_LabelSequence freeShapeLabels;
    _shapeTool->GetFreeShapes(freeShapeLabels);
    std::vector<ShapeIdPair> shapeIdPairs;
    for (auto label : freeShapeLabels){
        auto key = _labelKeyTool->keyFromLabel(label);
        auto id = ShapeIdFactory::create(key);
        auto shape = atId(id);
        shapeIdPairs.push_back(
            ShapeIdPair(id, shape)
        );
    }
    return shapeIdPairs;
}

#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

std::vector<ShapeIdPair> CoreShapeMap::subShapes(const ShapeId& id) const {
    std::vector<ShapeIdPair> shapeIdPairs;
    auto parentShape = atId(id);
    TopTools_IndexedMapOfShape subShapes;
    TopExp::MapShapes(parentShape, subShapes);
    for (int i = 1; i <= subShapes.Extent(); ++i) {
        const TopoDS_Shape& shape = subShapes(i);
        if (shape.IsSame(parentShape)) {
            continue;
        }   
        ShapeId shapeId = atShape(shape);
        if (shapeId.isValid()) {
            shapeIdPairs.push_back(ShapeIdPair(shapeId, shape));
        }
    }
    return shapeIdPairs;
}