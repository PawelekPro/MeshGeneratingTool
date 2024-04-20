#include "Geometry.h"

void GeometryCore::Geometry::importSTEP(const std::string& filePath, QWidget* progressBar){

    GeometryCore::STEPImporter importer;
    importer.import(filePath, progressBar);
    this->_solidsMap = importer.getSolidsMap();
    this->_facesMap = importer.getFacesMap();
    this->_edgesMap = importer.getEdgesMap();
    this->_vertexMap = importer.getVertexMap();

    for(auto shape:this->_vertexMap){
        this->_tagMap.tagShape(TopoDS::Vertex(shape.second));
    }
    for(auto shape:this->_edgesMap){
        this->_tagMap.tagShape(TopoDS::Edge(shape.second));
    }
    for(auto shape:this->_facesMap){
        this->_tagMap.tagShape(TopoDS::Face(shape.second));
    }
    for(auto shape:this->_solidsMap){
        this->_tagMap.tagShape(TopoDS::Solid(shape.second));
    }

    this->_partsActorMap = importer.getPartsActorMap();
    this->_facesActorMap = importer.getFacesActorMap();
    this->_edgesActorMap = importer.getEdgesActorMap();
};

void GeometryCore::Geometry::importSTL(const std::string& filePath, QWidget* progressBar){
    GeometryCore::STLImporter importer;
    importer.import(filePath, progressBar);
    this->_solidsMap = importer.getSolidsMap();
    for(auto shape:this->_solidsMap){
        this->_tagMap.tagShape(TopoDS::Solid(shape.second));
    }
    // this->_facesMap = std::move(importer.getFacesMap());
    // this->_edgesMap = std::move(importer.getEdgesMap());

    this->_partsActorMap = importer.getPartsActorMap();
    // this->_facesActorMap = std::move(importer.getFacesActorMap());
    // this->_edgesActorMap = std::move(importer.getEdgesActorMap());
};
