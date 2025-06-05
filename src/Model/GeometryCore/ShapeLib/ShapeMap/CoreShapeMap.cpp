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

using ShapeIdPair = ShapeMap::ShapeIdPair;

CoreShapeMap::CoreShapeMap(Handle(XCAFDoc_ShapeTool) aShapeTool) 
: ShapeMap(), _shapeTool(aShapeTool), _labelKeyTool(std::make_unique<LabelKeyTool>(aShapeTool)){}

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

    size_t labelTag = label.Tag();
    size_t parentLabelTag = 0;

    if (!_shapeTool->IsTopLevel(label)) {
        TDF_Label current = label;
        while (!current.IsRoot()) {
            current = current.Father();
            if (_shapeTool->IsTopLevel(current)) {
                parentLabelTag = current.Tag();
                break;
            }
        }
    }

    auto key = std::make_shared<ShapeKey>(labelTag, parentLabelTag);
    ShapeId id = ShapeIdFactory::create(key);
    return id;
}


std::vector<ShapeIdPair> CoreShapeMap::freeShapes() const {
    return std::vector<ShapeIdPair>();
}

std::vector<ShapeIdPair> CoreShapeMap::subShapes(const ShapeId& id) const {
    return std::vector<ShapeIdPair>();
}

const ShapeId CoreShapeMap::fromKey(const ShapeKey& aKey) const {
    return ShapeId::invalidId();
}