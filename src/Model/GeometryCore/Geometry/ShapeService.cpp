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

#include "ShapeService.hpp"
#include "STEPImporter.hpp"
#include "ShapeEvents.hpp"
#include "EventProgressIndicator.hpp"

ShapeService::ShapeService(
    const ModelSubject& aModelSubject, 
    ShapeCore& aShapeCore
) : _modelSubject(aModelSubject),
    _shapeCore(aShapeCore){
    
    _shapeCore.connectShapeAdded( [this](const ShapeId& aId) {
        publishNewShape(aId);});
    
    _shapeCore.connectShapeModified( [this](const ShapeId& aId) {
        publishShapeModified(aId);});

    _shapeCore.connectShapeRemoved( [this](const ShapeId& aId) {
        publishShapeRemoved(aId);});
}

bool ShapeService::importSTEP(const std::string& aFilePath) {
    STEPImporter importer;
    EventProgressIndicator indicator(_modelSubject);
    std::vector<std::pair<TopoDS_Shape, ShapeAttr>> shapes = 
        importer.importFile(aFilePath, indicator);
    
    for (const auto shapePair : shapes){
        _shapeCore.registerNewShape(shapePair.first);
    }
    return true;
}

bool ShapeService::importSTL(const std::string& aFilePath) {
    return true;
}

bool ShapeService::scaleShape(const ShapeId& aShapeId, double aScaleFactor) {
    return true;
}

void ShapeService::publishNewShape(const ShapeId& aId){
    ShapeAddedEvent event(aId);
    _modelSubject.publishEvent(event);
}

void ShapeService::publishShapeModified(const ShapeId& aId){
    ShapeModifiedEvent event(aId);
    _modelSubject.publishEvent(event);
}

void ShapeService::publishShapeRemoved(const ShapeId& aId){
    ShapeRemovedEvent event(aId);
    _modelSubject.publishEvent(event);
}