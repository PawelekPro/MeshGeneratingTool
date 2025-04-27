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

#include "LabelShapeMap.hpp"
#include "LabelShapeKey.hpp"

LabelShapeMap::LabelShapeMap(Handle(XCAFDoc_ShapeTool) aShapeTool) 
: ShapeMap(), _shapeTool(aShapeTool){}

TDF_Label LabelShapeMap::idLabel(const ShapeId& aId) {
    TDF_Label label = ShapeIdFactory::getKey<LabelShapeKey>(aId).label();
    return label;
}

ShapeId LabelShapeMap::shapeId(const TDF_Label& aLabel) const {
    if (_shapeTool->IsTopLevel(aLabel)){
        return ShapeIdFactory::create(std::make_unique<LabelShapeKey>(aLabel));
    }
  
    TDF_Label parentLabel = aLabel.Father();
    if (!_shapeTool->IsShape(parentLabel)){
        std::string err = "Shape is not top level but lacks parent label.";
        throw std::runtime_error(err);
    }

    ShapeId parentId = ShapeIdFactory::create(
        std::make_unique<LabelShapeKey>(parentLabel)
    );
    
    return ShapeIdFactory::create(
        std::make_unique<LabelShapeKey>(aLabel),
        std::make_shared<ShapeId>(parentId)
    );
}

bool LabelShapeMap::containsId(const ShapeId& id) const {
    TDF_Label label = idLabel(id);
    return _shapeTool->IsShape(label);
};

bool LabelShapeMap::containsShape(const TopoDS_Shape& shape) const {
    TDF_Label label;
    return _shapeTool->Search(shape, label);
};

const TopoDS_Shape LabelShapeMap::atId(const ShapeId& id) const {
    TDF_Label label = idLabel(id);
    TopoDS_Shape shape = _shapeTool->GetShape(label);
    if (shape.IsNull()){
        throw std::runtime_error("Shape not found in map.");
    } 
    return shape;
};

const ShapeId LabelShapeMap::getId(const TopoDS_Shape& shape) const {
    TDF_Label label; 
    bool found = _shapeTool->Search(shape, label);
    if ( !found ){
        throw std::runtime_error("Shape not found in map.");
    }
    return shapeId(label);
};

const ShapeId LabelShapeMap::registerTopLevelShape(
    const TopoDS_Shape& shape
) {
    TDF_Label label = _shapeTool->AddShape(shape);
    return shapeId(label);
};

const ShapeId LabelShapeMap::registerSubShape(
    const TopoDS_Shape& shape, 
    const ShapeId& parentId
) {
    TDF_Label parentLabel = idLabel(parentId);
    TDF_Label label = _shapeTool->AddSubShape(parentLabel, shape);
    return shapeId(label);
};

bool LabelShapeMap::removeShape(const ShapeId& id) {
    if (!containsId(id)) {
        return false;
    }
    TDF_Label label = idLabel(id);
    return _shapeTool->RemoveShape(label);
};

bool LabelShapeMap::updateShape(
    const ShapeId& id,
    const TopoDS_Shape& shape
) {
    if (!containsId(id)) {
        return false;
    }
    TDF_Label label = idLabel(id);
    _shapeTool->SetShape(label, shape);
    return true;
};