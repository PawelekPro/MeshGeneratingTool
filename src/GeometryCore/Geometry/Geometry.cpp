#include "Geometry.h"

void GeometryCore::Geometry::importSTEP(const std::string& filePath, QWidget* progressBar){

    GeometryCore::STEPImporter importer;
    importer.import(filePath, progressBar);
    this->_partsMap = std::move(importer.getPartsMap());
    this->_facesMap = std::move(importer.getFacesMap());
    this->_edgesMap = std::move(importer.getEdgesMap());

    this->_partsActorMap = std::move(importer.getPartsActorMap());
    this->_facesActorMap = std::move(importer.getFacesActorMap());
    this->_edgesActorMap = std::move(importer.getEdgesActorMap());
};

void GeometryCore::Geometry::importSTL(const std::string& filePath, QWidget* progressBar){
    GeometryCore::STLImporter importer;
    importer.import(filePath, progressBar);
    this->_partsMap = std::move(importer.getPartsMap());
    // this->_facesMap = std::move(importer.getFacesMap());
    // this->_edgesMap = std::move(importer.getEdgesMap());

    this->_partsActorMap = std::move(importer.getPartsActorMap());
    // this->_facesActorMap = std::move(importer.getFacesActorMap());
    // this->_edgesActorMap = std::move(importer.getEdgesActorMap());
};