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
    return _idShapeMap.contains(aId);
};

bool StdShapeMap::containsShape(const TopoDS_Shape& aShape) const {
    return _shapeIdMap.contains(aShape);
};

const TopoDS_Shape StdShapeMap::atId(const ShapeId& aId) const {
    return _idShapeMap.at(aId); 
}

const ShapeId StdShapeMap::getId(const TopoDS_Shape& aShape) const {
    return _shapeIdMap.at(aShape);
}

const ShapeId StdShapeMap::registerTopLevelShape(const TopoDS_Shape& aShape) {
    const ShapeId id = _idRegistry.generateTopLevelId(aShape);

    bool idShapeInserted = _idShapeMap.insert({id, aShape}).second;
    bool shapeIdInserted = _shapeIdMap.insert({aShape, id}).second;

    if (!idShapeInserted || !shapeIdInserted) {
        throw std::runtime_error("Could not insert shape into the map!");
    }

    _idRegistry.bindTopLevelId(id);
    return id;
}

const ShapeId StdShapeMap::registerSubShape(
    const TopoDS_Shape& aShape, 
    const ShapeId& aParentId,
    int aSubShapeid
) {
    const ShapeId id = _idRegistry.generateSubId(aShape, aParentId, aSubShapeid);

    bool idShapeInserted = _idShapeMap.insert({id, aShape}).second;
    bool shapeIdInserted = _shapeIdMap.insert({aShape, id}).second;

    if (!idShapeInserted || !shapeIdInserted) {
        throw std::runtime_error("Could not insert shape into the map!");
    }
    
    _idRegistry.bindTopLevelId(id);
    return id;
}

bool StdShapeMap::removeShape(const ShapeId& id) {
    auto idIt = _idShapeMap.find(id);
    if (idIt == _idShapeMap.end()) {
        return false;
    }

    const TopoDS_Shape& shape = idIt->second;

    auto shapeIt = _shapeIdMap.find(shape);
    if (shapeIt == _shapeIdMap.end() || shapeIt->second != id) {
        throw std::runtime_error("Inconsistent map state!");
        return false;
    }

    _idShapeMap.erase(idIt);
    _shapeIdMap.erase(shapeIt);

    return true;
}

bool StdShapeMap::updateShape(
    const ShapeId& aId,
    const TopoDS_Shape& aShape
) {
    auto idIt = _idShapeMap.find(aId);
    if (idIt == _idShapeMap.end()) {
        return false;
    }

    const TopoDS_Shape& oldShape = idIt->second;

    auto shapeIt = _shapeIdMap.find(oldShape);
    if (shapeIt == _shapeIdMap.end() || shapeIt->second != aId) {
        throw std::runtime_error("Inconsistent map state!");
    }

    _idShapeMap.erase(idIt);
    _shapeIdMap.erase(shapeIt);

    bool idShapeInserted = _idShapeMap.insert({aId, aShape}).second;
    bool shapeIdInserted = _shapeIdMap.insert({aShape, aId}).second;

    if (!idShapeInserted || !shapeIdInserted) {
        _idShapeMap.insert({aId, oldShape});
        _shapeIdMap.insert({oldShape, aId});
        return false;
    }
    return true;
}
