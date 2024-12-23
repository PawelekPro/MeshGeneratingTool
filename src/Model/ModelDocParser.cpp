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

#include "ModelDocParser.hpp"

ModelDocParser::ModelDocParser(Model& aModel) : _model(aModel),
                                                _doc(DocumentHandler::getInstance()){}

void ModelDocParser::applyElementSizings(){
    QList<QDomElement> sizingElements = _doc.getElementsByType(ItemTypes::Mesh::ElementSizing);
    for(auto sizingElem : sizingElements){
        std::pair<std::vector<int>, double> sizing = parseElementSizing(sizingElem);
        _model.addSizing(sizing.first, sizing.second);
    }
}

std::pair<std::vector<int>, double> ModelDocParser::parseElementSizing(const QDomElement& aSizingElement){
    //TODO: All those error checks should be in propertyValue - here they are redundant and clutter the code
    QString sizeString, tagsString, shapeTypeString;
    try{sizeString = _doc.getPropertyValue(aSizingElement, "elementSize");}
    catch(const std::runtime_error& e){
        qWarning() << e.what() << "Skipping this meshSizing...";
    }
    try{tagsString = _doc.getPropertyValue(aSizingElement, "selectedTags");}
    catch(const std::runtime_error& e){
        qWarning() << e.what() << "Skipping this meshSizing...";
    }
    try{shapeTypeString = _doc.getPropertyValue(aSizingElement, "selectionType");}
    catch(const std::runtime_error& e){
        qWarning() << e.what() << "Skipping this meshSizing...";
    }
    if(sizeString.isEmpty()){
        qWarning() << "Element size is empty in " << aSizingElement.attribute("name") << " skipping...";
    }
    if(tagsString.isEmpty()){
        qWarning() << "selectedTags value is empty in " << aSizingElement.attribute("name") << " skipping...";
    }
    if(shapeTypeString.isEmpty()){
        qWarning() << "selectedTags value is empty in " << aSizingElement.attribute("name") << " skipping...";
    }
    double size = sizeString.toDouble();
    std::vector<int> verticesTags;
    GeometryCore::EntityType selectionType = GeometryCore::EntityType::Vertex;
    if(shapeTypeString == "Vertex"){
        selectionType = GeometryCore::EntityType::Vertex;
    } else if(shapeTypeString == "Edge"){
        selectionType = GeometryCore::EntityType::Edge;
    } else if(shapeTypeString == "Face"){
        selectionType = GeometryCore::EntityType::Face;
    } else if(shapeTypeString == "Solid"){
        selectionType = GeometryCore::EntityType::Solid;
    } else {
        qWarning() << "Selection type: " << shapeTypeString << " does not exist, assuming Vertex";
    }
    QStringList tagsList = tagsString.split(',', Qt::SkipEmptyParts);
    for (const QString& tagString : tagsList) {
        int shapeTag = tagString.toInt();
        const TopoDS_Shape& shape = _model.geometry.getTagMap().getShape(selectionType, shapeTag);
        std::vector<int> shapeVerticesTags = _model.geometry.getShapeVerticesTags(shape);
        verticesTags.insert(verticesTags.end(), shapeVerticesTags.begin(), shapeVerticesTags.end());
    }   
    return std::pair<std::vector<int>, double>(verticesTags, size);
}

void ModelDocParser::applyMeshSettings(){
    double minSize;
    double maxSize;
    QDomElement meshElement = _doc.getRootElement(ItemTypes::Root::Mesh);
    try {
        QString minValue = _doc.getPropertyValue(meshElement, "minElementSize");
        minSize = minValue.toDouble();
    } catch (const std::runtime_error& e) {
        qWarning() << e.what() << " Setting minElementSize to default 0.1";
        minSize = 0.1;
    }

    try {
        QString maxValue = _doc.getPropertyValue(meshElement, "maxElementSize");
        maxSize = maxValue.toDouble();
    } catch (const std::runtime_error& e) {
        qWarning() << e.what() << " Setting maxElementSize to default 1";
        maxSize = 1;
    }

    std::cout << "settting sizes: " << minSize << " " << maxSize;

    gmsh::option::setNumber("Mesh.MeshSizeMin", minSize);
    gmsh::option::setNumber("Mesh.MeshSizeMax", maxSize);
}