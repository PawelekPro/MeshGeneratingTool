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

#include "StdShapeMap.hpp"

bool StdShapeMap::containsId(const ShapeId& aId) const {
    bool isInIdShapeMap = _idShapeMap.contains(aId);
    if (!isInIdShapeMap){
        return false;
    }
    
    TopoDS_Shape shape = _idShapeMap.at(aId);
    bool isInShapeIdMap = _shapeIdMap.contains(shape);

    if (!isInShapeIdMap){
        throw InvalidMapState("Invalid Map state.");
    }
    
    ShapeId fetchedId = _shapeIdMap.at(shape);
    
    if (fetchedId != aId){
        throw InvalidMapState("Invalid Map state.");
    }

    return true;
}

bool StdShapeMap::containsShape(const TopoDS_Shape& aShape) const {
    bool isInShapeIdMap = _shapeIdMap.contains(aShape);

    if (!isInShapeIdMap){
        return false;
    }
    
    ShapeId id = _shapeIdMap.at(aShape);
    
    bool isInIdShapeMap = _idShapeMap.contains(id);
    if (!isInIdShapeMap){
        throw InvalidMapState("Could not find id that exists in shapeIdMap.");
    }
    
    TopoDS_Shape fetchedShape = _idShapeMap.at(id);
    
    if (fetchedShape != aShape){
        throw InvalidMapState("Mismatch between shapeIdMap and idShapeMap.");
    }
    return true;
};

const TopoDS_Shape StdShapeMap::atId(const ShapeId& aId) const {
    return _idShapeMap.at(aId); 
}

const ShapeId StdShapeMap::getId(const TopoDS_Shape& aShape) const {
    return _shapeIdMap.at(aShape);
}

bool StdShapeMap::insert(const std::pair<ShapeId, TopoDS_Shape>& aIdShapePair){
    const ShapeId& id = aIdShapePair.first;
    const TopoDS_Shape& shape = aIdShapePair.second;

    if (containsId(id) || containsShape(shape)){
        return false;
    }

    bool idShapeInserted = _idShapeMap.insert({id, shape}).second;
    bool shapeIdInserted = _shapeIdMap.insert({shape, id}).second;

    if (!idShapeInserted || !shapeIdInserted) {
        if (idShapeInserted){
            _idShapeMap.erase(id);
        }
        if (shapeIdInserted){
            _shapeIdMap.erase(shape);
        }
        throw InvalidMapState("Insert operation failed for one of the maps.");
    }
    _idRegistry.bindId(id);
    return true;
}

const ShapeId StdShapeMap::registerTopLevelShape(const TopoDS_Shape& aShape) {
    const ShapeId id = _idRegistry.generateTopLevelId(aShape);
    if (insert({id, aShape})){
        return id;
    } else {
        spdlog::warn("Shape already registered. Returning invalid id.");
        return *ShapeId::InvalidId();
    }
}

const ShapeId StdShapeMap::registerSubShape(
    const TopoDS_Shape& aShape, 
    const ShapeId& aParentId
) {
    const ShapeId id = _idRegistry.generateSubId(aShape, aParentId);
    if (insert({id, aShape})){
        return id;
    } else {
        spdlog::warn("Shape already registered. Returning invalid id.");
        return *ShapeId::InvalidId();
    }
}

bool StdShapeMap::removeShape(const ShapeId& aId) {
    if (!containsId(aId)) {
        spdlog::warn("Shape ID not found, cannot remove shape.");
        return false;
    }

    TopoDS_Shape shape = _idShapeMap.at(aId);
    if (_idShapeMap.erase(aId) == 0) {
        spdlog::warn("Failed to remove shape ID from _idShapeMap.");
        return false;
    }

    if (_shapeIdMap.erase(shape) == 0) {
        spdlog::warn("Failed to remove shape from _shapeIdMap.");
        _idShapeMap.insert({aId, shape});
        return false;
    }

    return true;
}
bool StdShapeMap::updateShape(
    const ShapeId& aId,
    const TopoDS_Shape& aShape
) {
    if (!removeShape(aId)){
        return false;
    }
    return insert({aId, aShape});
}
