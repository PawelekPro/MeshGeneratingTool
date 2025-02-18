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

#include "ShapeManager.hpp"

ShapeManager::ShapeManager(OcafDoc& aDoc) : 
    _ocaf(aDoc),
    _shapeMap(aDoc),
    _shapeFactory(ShapeFactory(aDoc)){}

std::vector<ShapeId> ShapeManager::importShapes(
    const GeometryImporter& aImporter){
    std::vector<ShapeId> vec;
    return vec;
};

ShapeId ShapeManager::registerNewShape(const TopoDS_Shape& aShape){
    ShapeId shapeId = _shapeMap.generateShapeId();
    TDF_Label label = _ocaf.addNewShape(aShape);
    GeoShape geoShape = _shapeFactory.createNewShape(aShape, shapeId, label);
    _shapeMap.registerShape(geoShape);
    return shapeId;
}

bool ShapeManager::removeShape(const ShapeId& aShapeId){  
    std::pair<bool, GeoShape> shapePair = getShape(shapeId);
    if(!shapePair.first){
        return shapePair.first;
    }

    _ocafDoc.removeShape(shepPair.second.label());
    _shapeMap.remove(aShapeId);
    
    return true;
};

std::pair<bool, GeoShape> ShapeManager::getShape(const ShapeId& aShapeId) {
    std::pair<bool, GeoShape> shapePair(false, _shapeFactory.emptyShape());
    auto it = _shapeMap.find(aShapeId);
    if (it != _shapeMap.end()) {
        shapePair.first = true;
        shapePair.second = it->second;
    }
    return shapePair;
}

AttributeEditor.renameShape(ShapeId, newName){
    geoShape = ShapeManager.getShape(ShapeId);
    OcafAttributes.setAttribute(geoShape.label, OcafAttributes::Name, newName);

}

SolidEditor.scaleShape(ShapeId, scaleFactor){;
    geoShape = ShapeManager.getShape(ShapeId);
    TopoDS_SHape topoSHape = geoShape.shape();
    
    scaledShape = Brep_Tools.scale(topoShape, scale);
    OcafShapes.shapeEdited( geoShape.label, scaledShape ); 

}

BooleanEditor.mergeShapes(ShapeId, ShapeId){
    geoShapeParent = ShapeManager.getShape(ShapeId);
    geoShape = ShapeManager.getShape(ShapeId);

    merged = BrepTools.merged;
    OcafShapes.shapeEdited( geoShapeParent.label, merged );
    OcafShapes.shapeRemoved( geoShape.label )
}

GeometryView{
    




}

GeometryActions{

    editShape(ShapeId){
        Command command = ShapeCommandFactory.createEditShape(shapeId);
        geometryCommandStack.execute(command);
    }
}

EditCommand{

    execute(){
    
    shape = shapeMap.getShape(shapeId);
    editedShape = ShapeTools::editShape(shape);
    
    ShapeStateManager.commitShapeEdit(shapeId, editedShape);
    
    ShapeEditEvent shapeEvent(shapeId);
    ShapeSubject.publishEvent(shapeEvent);
    }
    
    undo(){
        ShapeStateManager.undo();
        ShapeSubject.publishEvent(shapeEvent);
    }
}

ShapeMap{
    shape getShape(shapeId) = 0;
    bool addShape(shape, shapeId) = 0;
    bool removeShape(shapeId) = 0;
    shapeId genShapeId() = 0;
}
Shubject{
    publishEvent() = 0
}
ShapeStateManager{

    commitShapeEdit(ShapeId, editedShape) = 0;
    undo() = 0;
}

CachedShapeMap {
    shape getShape(shapeId){
        return shmap.at(shapeId);
    }
    bool addShape(shape, shapeId)
    ... etc
}

OcafShapeStateManager : ShapeStateManager{

    commitShapeEdit (shapeId, editedShape){
        label = labelMap.getLabel(shapeId);
        ocafDoc.NewCommand();
        ocafDoc.updateShape(label, editedShape);
        ocafDoc.CmmitCommand();
    }

    undo(){
        ocafDoc.undo();
    }
}