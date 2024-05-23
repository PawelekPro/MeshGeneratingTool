#include "Geometry.h"

GeometryCore::Geometry::Geometry(){};
GeometryCore::Geometry::~Geometry(){};

void GeometryCore::Geometry::importSTEP(const std::string& filePath, QWidget* progressBar){

    GeometryCore::STEPImporter importer;
    importer.import(filePath, progressBar);
    this->_shapesMap = std::move(importer.getPartsMap());
    for(auto shape : _shapesMap){
        this->_tagMap.tagEntities(shape.second);
    }
};
void GeometryCore::Geometry::importSTL(const std::string& filePath, QWidget* progressBar){
    GeometryCore::STLImporter importer;
    importer.import(filePath, progressBar);
    this->_shapesMap = std::move(importer.getPartsMap());
    for(auto shape : _shapesMap){
        this->_tagMap.tagEntities(shape.second);
    }
};
