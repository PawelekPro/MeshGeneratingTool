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
#include "ShapeCore.hpp"
#include "STEPImporter.hpp"
#include "STLImporter.hpp"
#include "MessageProgressIndicator.hpp"
#include "ShapeTools.hpp"


ShapeService::ShapeService(
    MessageBus& aMessageBus, 
    std::shared_ptr<ShapeCore> aShapeCore
) : _shapeCore(aShapeCore),
    _messageBus(aMessageBus),
    _signalWrapper(std::make_shared<ShapeSignalWrapper>(aMessageBus)){
        connectToShapeCore();
    }        

void ShapeService::importShapes(
    const ShapeImporter& aImporter, 
    const std::string& aFilePath
){
    MessageProgressIndicator progressIndicator(_messageBus);
    auto doc = aImporter.import(
        aFilePath,
        progressIndicator
    );
    _shapeCore->importDocument(doc);
};

void ShapeService::removeShape(const ShapeId& aShapeId){
    _shapeCore->removeShape(ShapeIdFactory::getKey(aShapeId));
};

void ShapeService::scaleShape(const ShapeId& aShapeId, float aScaleFactor){
    auto shape = _shapeCore->shapeMap()->atId(aShapeId);
    auto scaledShape = ShapeTools::scaleShape(shape, aScaleFactor);
    _shapeCore->updateShape({ShapeIdFactory::getKey(aShapeId), scaledShape});
};

void ShapeService::connectToShapeCore() {
    _shapeCore->attachObserver(_signalWrapper);
};