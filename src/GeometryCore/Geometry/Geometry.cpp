#include "Geometry.h"

void GeometryCore::Geometry::importSTEP(const std::string& filePath){

    GeometryCore::STEPImporter importer;

    
    this->_partsMap = std::move(importer.getPartsMap());
    this->_facesMap = std::move(importer.getPartsMap());
    this->_edgesMap = std::move(importer.getPartsMap());

    this->_partsActorMap = std::move(importer.getPartsActorMap());
    this->_facesActorMap = std::move(importer.getFacesActorMap());
    this->_edgesActorMap = std::move(importer.getEdgesActorMap());
};

void GeometryCore::Geometry::importSTL(const std::string& filePath){
    
};