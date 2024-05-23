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

std::vector<int> GeometryCore::Geometry::getShapeVerticesTags(const TopoDS_Shape& shape){
    std::vector<int> vertexTags;
    int vertex_tag;
    TopExp_Explorer shapeExplorer(shape, TopAbs_VERTEX);
    for(; shapeExplorer.More(); shapeExplorer.Next()) {
        const TopoDS_Vertex& vertex = TopoDS::Vertex(shapeExplorer.Current());
        vertex_tag = _tagMap.getTag(vertex);
        vertexTags.push_back(vertex_tag);
    }
    std::sort(vertexTags.begin(), vertexTags.end());
    vertexTags.erase(std::unique(vertexTags.begin(), vertexTags.end()), vertexTags.end());
    return vertexTags;
}

