#include "Geometry.h"

void GeometryCore::Geometry::importStep(const std::string& filePath){
    this->_importer = std::make_unique<STEPImporter>();
}





void GeometryCore::Geometry::importSTL(const std::string& filePath){
    
}