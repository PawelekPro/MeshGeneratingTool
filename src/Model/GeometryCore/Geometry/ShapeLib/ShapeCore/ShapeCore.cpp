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

#include "ShapeCore.hpp"

boost::signals2::connection ShapeCore::connectShapeAdded(
    const std::function<void(const ShapeId&)>& slot
) {
    return _publisher.shapeAddedSignal().connect(slot); 
}

boost::signals2::connection ShapeCore::connectShapeRemoved(
    const std::function<void(const ShapeId&)>& slot
) {
    return _publisher.shapeRemovedSignal().connect(slot);
}

boost::signals2::connection ShapeCore::connectShapeModified(
    const std::function<void(const ShapeId&)>& slot
) {
    return _publisher.shapeModifiedSignal().connect(slot);
}

const ShapeId ShapeCore::registerTopLevelShapeToShapeMap(
    const TopoDS_Shape& aShape
) {
    return _shapeMap->registerTopLevelShape(aShape);
}

const ShapeId ShapeCore::registerSubShapeToShapeMap(
    const TopoDS_Shape& aShape, 
    const ShapeId& aParentId
) {
    return _shapeMap->registerSubShape(aShape, aParentId);
}

bool ShapeCore::removeShapeFromShapeMap(const ShapeId& id) {
    return _shapeMap->removeShape(id);
}

bool ShapeCore::updateShapeInShapeMap(
    const ShapeId& id,
    const TopoDS_Shape& aShape
) {
    return _shapeMap->updateShape(id, aShape);
}

std::shared_ptr<const ShapeMap> ShapeCore::shapeMap() const {
    return _shapeMap;
}