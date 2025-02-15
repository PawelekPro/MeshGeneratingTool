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

#ifndef SHAPEFACTORY_HPP
#define SHAPEFACTORY_HPP

#include "GeoShape.hpp"

class OcafDoc;
class ShapeManager;
class ShapeId;

class ShapeFactory {
   friend class ShapeManager;

    private:
    
    ShapeFactory(OcafDoc& aOcaf);
    GeoShape createShape(const ShapeId& aShapeId,
        const TopoDS_Shape& aShape,
        const std::string& aInitName);

    OcafDoc& _ocaf;
};


#endif


//  ImportSTEPCommand (filePath) {
//     execute(){
//         std::map<TDF_Label> importedShapesLabels = OcafDoc.import((filePath)record = true);
//         std::vector<GeoShape> newSHapes;
//         for shapeLabel in importedShapesLabels:
//             GeoShape geoshape = ShapeManager.registerNewShape(shapeLabel);
//             newShapes.append(geoshape);
//         geoSubject.publish(ImportedShapesEvent(newShapes)); 
//         importedShapes = newShapes;
//     }

//     undo(){
//         shapeManager::removeShapes(importedShapes);
//         ocafDoc.undo();
//         shapeManager::verifyOcafLabels();
//         geoSubject.publish(removedSHapesEvent(importedShapes));
//     }
// }