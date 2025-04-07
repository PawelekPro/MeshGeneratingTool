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

#include "OcafShapeMap.hpp"

bool OcafShapeMap::containsId(const ShapeId& aId) const {
    const auto it = _idLabelMap.find(aId);
    if(it == _idLabelMap.end()){
        return false;
    } else {
        return true;
    }
};

bool OcafShapeMap::containsShape(const TopoDS_Shape& aId) const {
    return false;
};

const TopoDS_Shape OcafShapeMap::atId(const ShapeId& aId) const {
    // const auto it = _idLabelMap.find(aId);
    // if (it != _idLabelMap.end()) {
    //     if (auto shape = _ocafDoc.getShape(it->second)) {
    //         return *shape;
    //     }
    // }
    throw std::runtime_error("ShapeId not found");
    return TopoDS_Shape();
}

const ShapeId OcafShapeMap::getId(const TopoDS_Shape& aShape) const {
    // if (auto label = _ocafDoc.findLabel(aShape)) {
    //     for (const auto& [id, lbl] : _idLabelMap) {
    //         if (lbl == *label) {
    //             return id;
    //         }
    //     }
    // }
    return ShapeId(0, ShapeType::Invalid);
}

const ShapeId OcafShapeMap::registerShape(const TopoDS_Shape& aShape){
    // if(auto id = getId(aShape)){
    //     return id;
    // }
    return ShapeId(0, ShapeType::Invalid);
};

bool OcafShapeMap::removeShape(const ShapeId& id){
    return false;
};    

bool OcafShapeMap::updateShape(
    const ShapeId& id,
    const TopoDS_Shape& shape){
    return false;
};