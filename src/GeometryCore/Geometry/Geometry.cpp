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

std::vector<std::reference_wrapper<const TopoDS_Vertex>> GeometryCore::getShapeVertices(const TopoDS_Shape& shape){
    std::vector<std::reference_wrapper<const TopoDS_Vertex>> vertices;
    for (TopExp_Explorer vertexExplorer(shape, TopAbs_VERTEX);
        vertexExplorer.More();
        vertexExplorer.Next()){
            const TopoDS_Vertex& vertex = TopoDS::Vertex(vertexExplorer.Current());
            vertices.push_back(vertex);
            }
    return vertices;
}

