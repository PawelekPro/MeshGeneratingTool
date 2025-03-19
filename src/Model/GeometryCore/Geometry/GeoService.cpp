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

#include "GeoService.hpp"
#include "ShapeEvents.hpp"
#include "STEPImporter.hpp"
#include "STLImporter.hpp"

GeoService::GeoService(
    const ModelSubject& aModelSubject,
    GeoState& aGeoState
) :
_modelSubject(aModelSubject),
_geoState(aGeoState){}

std::map<std::string, TopoDS_Shape> GeoService::importSTEP(
    const std::string& aFilePath
){
    GeometryCore::STEPImporter importer; 
    importer.import(aFilePath, _modelSubject);
    
    std::map<std::string, TopoDS_Shape> shapes = importer.getPartsMap();
    
    _geoState.addShapes(shapes); 

    NewShapesEvent event(shapes);
    _modelSubject.publishEvent(event);
    return shapes;
}

std::map<std::string, TopoDS_Shape> GeoService::importSTL(
    const std::string& aFilePath
){
    GeometryCore::STLImporter importer; 
    importer.import(aFilePath, _modelSubject);
    std::map<std::string, TopoDS_Shape> shapes = importer.getPartsMap();
    _geoState.addShapes(shapes); 
    NewShapesEvent event(shapes);
    _modelSubject.publishEvent(event);
    return shapes;
}


void GeoService::removeShapes(const std::vector<TopoDS_Shape>& aShapes){
    _geoState.removeShapes(aShapes);
    return;
}
