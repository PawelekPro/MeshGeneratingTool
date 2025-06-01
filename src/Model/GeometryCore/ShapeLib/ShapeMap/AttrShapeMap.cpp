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

#include "AttrShapeMap.hpp"

using ShapeIdPair = ShapeMap::ShapeIdPair;

AttrShapeMap::AttrShapeMap(Handle(XCAFDoc_ShapeTool) aShapeTool) 
: ShapeMap(), _shapeTool(aShapeTool) {}

bool AttrShapeMap::containsId(const ShapeId& id) const {
    TDF_Label shapeLabel = findLabel(ShapeIdFactory::getKey<LabelTagKey>(id));
    TopoDS_Shape shape = _shapeTool->GetShape(shapeLabel);
    return !shape.IsNull();
}

bool AttrShapeMap::containsShape(const TopoDS_Shape& shape) const {
    TDF_Label label;
    return _shapeTool->Search(shape, label);
}

const TopoDS_Shape AttrShapeMap::atId(const ShapeId& id) const {
    TDF_Label shapeLabel = findLabel(ShapeIdFactory::getKey<LabelTagKey>(id));
    TopoDS_Shape shape = _shapeTool->GetShape(shapeLabel);
    return shape;
}

const ShapeId AttrShapeMap::atShape(const TopoDS_Shape& shape) const {
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

    auto key = std::make_unique<LabelTagKey>(labelTag, parentLabelTag);
    ShapeId id = ShapeIdFactory::create(std::move(key));
    return id;
}


std::vector<ShapeIdPair> AttrShapeMap::freeShapes() const {
    return std::vector<ShapeIdPair>();
}

std::vector<ShapeIdPair> AttrShapeMap::subShapes(const ShapeId& id) const {
    return std::vector<ShapeIdPair>();
}

const ShapeId AttrShapeMap::fromKey(const ShapeKey& aKey) const {
    return ShapeId::invalidId();
}

#include "TDF_Tool.hxx"
TDF_Label AttrShapeMap::findLabel(const LabelTagKey& aKey) const {
    size_t labelTag = aKey.labelTag();
    size_t parentLabelTag = aKey.parentLabelTag();

    TDF_Label shapesLabel = _shapeTool->Label();

    if (parentLabelTag == 0) {
        return shapesLabel.FindChild(static_cast<Standard_Integer>(labelTag));
    } else {
        TDF_Label parentLabel = shapesLabel.FindChild(static_cast<Standard_Integer>(parentLabelTag));
        if (parentLabel.IsNull()) {
            return TDF_Label();
        }
        return parentLabel.FindChild(static_cast<Standard_Integer>(labelTag));
    }
}