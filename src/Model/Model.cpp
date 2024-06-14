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



#include "Model.h"


Model::Model(DocumentHandler* documentHandler, std::string modelName) : _modelName(modelName), _documentHandler(documentHandler) {
        gmsh::model::add(_modelName);
        this->geometry = GeometryCore::Geometry();
        this->mesh = MeshCore::Mesh();
        };
Model::~Model(){
    gmsh::finalize();
}

void Model::addShapesToModel(const GeometryCore::PartsMap& shapesMap) { 
    for (const auto& it : shapesMap) {
        const auto& shape = it.second;
        const void *shape_ptr = &shape;
        std::vector< std::pair<int, int>> outDimTags;
        gmsh::model::occ::importShapesNativePointer(shape_ptr, outDimTags);
    }
    gmsh::model::occ::synchronize();
}

void Model::addSizing(const std::vector<std::reference_wrapper<const TopoDS_Shape>> selectedShapes){
    for(const auto& shape : selectedShapes){
        const TopoDS_Shape& shapeRef =  shape.get();
        std::vector<int> vertexTags = geometry.getShapeVerticesTags(shape);
        mesh.addSizing(vertexTags, 0.1);
    }
}
void Model::addSizing(const std::vector<int>& verticesTags, double size){
    mesh.addSizing(verticesTags, size);
}

void Model::meshSurface() {
    applyMeshSettings();
    mesh.generateSurfaceMesh();
}
void Model::meshVolume(){
    applyMeshSettings();
    mesh.generateVolumeMesh();
}
            
vtkSmartPointer<vtkActor> Model::getMeshActor(){
    return mesh.getMeshActor();
}

void Model::importSTL(const std::string& filePath, QWidget* progressBar){
    geometry.importSTL(filePath, progressBar);
    GeometryCore::PartsMap shapesMap = geometry.getShapesMap();
    addShapesToModel(shapesMap);
}

void Model::importSTEP(const std::string& filePath, QWidget* progressBar){
    geometry.importSTEP(filePath, progressBar);
    GeometryCore::PartsMap shapesMap = geometry.getShapesMap();
    addShapesToModel(shapesMap);
}

void Model::initializeGmsh() {
    static bool gmshInitialized = false;
    if (!gmshInitialized) {
        try {
            gmsh::initialize();
            gmshInitialized = true;
        } catch (const std::exception &e) {
            std::cerr << "Error initializing Gmsh: " << e.what() << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown error initializing Gmsh" << std::endl;
            throw;
        }
    }
}


void Model::applyMeshSettings(){
    applyMeshGlobalSize();
    applyMeshSizings();
};


void Model::applyMeshGlobalSize(){
    //TODO move this logic somewhere else, according to solution to TODO from setMeshSizign Suggestions:
    // 1. some kind of method to fetch values from element so therese no need to constantly allocate nodelists and raw loops.
    double minSize = 0.1;
    double maxSize = 1;
    QDomElement meshElement = this->_documentHandler->getRootElement(DocumentHandler::RootTag::Mesh);
    QDomElement meshProps = this->_documentHandler->getElementsPropertiesElement(meshElement);
    QDomNodeList propertiesList = meshProps.childNodes();
    for(int i = 0; i < propertiesList.size(); i++){
        QDomElement meshProp = propertiesList.at(i).toElement();
        if (meshProp.attribute("name") == "minElementSize"){
            minSize = meshProp.text().toDouble();
        }else if(meshProp.attribute("name") == "minElementSize"){
            maxSize = meshProp.text().toDouble();
        }
    }
    gmsh::option::setNumber("Mesh.MeshSizeMin", minSize);
    gmsh::option::setNumber("Mesh.MeshSizeMax", maxSize);
};


void Model::applyMeshSizings(){
    //TODO move this logic somewhere else. Suggestions:
        // 1.1 Implement a method that returns a list of items which elements have specific tagname (in this case MeshSzigin) in TreeStructure.
        // 1.2 Get values of selectedTags and element size from PropertiesModel assigned to an item
        // OR
        // 2.1 Use a method getElementByTag from handler to get list of meshSizings
        // 2.2 Implement a method in document handler that returns a value of property (input(element, property name), output QString),
        // Use the TreeStructure/DocumentHandler method to get data and then get values of ElementSize and verticesTags here!
        // Operations like those below should not be in the event handler!!!
    QList<QDomElement> sizingElements = this->_documentHandler->getElementsByTag(DocumentHandler::EntryTag::MeshSizing);
    for(auto sizingElem : sizingElements){
        std::vector<int> currentTags;
        double size;
        QDomElement propertiesElem = this->_documentHandler->getElementsPropertiesElement(sizingElem);
        QDomNodeList propertiesList = propertiesElem.childNodes();
        for(int i = 0; i < propertiesList.size(); i++){
            QDomElement propertyElem = propertiesList.at(i).toElement();
            if (propertyElem.attribute("name") == "elementSize"){
                size = propertyElem.text().toDouble();
            }
            if (propertyElem.attribute("name") == "selectedTags"){
                QString tags = propertyElem.text();
                QStringList tagsList = tags.split(',', Qt::SkipEmptyParts);
                for (const QString& tag : tagsList) {
                    currentTags.push_back(tag.toInt());
                }
            }
        }
        this->addSizing(currentTags, size);
    }
};