#include "TagMap.h"

GeometryCore::TagMap::TagMap() {
    std::fill(_maxEntityTags.begin(), _maxEntityTags.end(), 0);
}

int GeometryCore::TagMap::getMaxTag(EntityType type){
    return this->_maxEntityTags.at(static_cast<int>(type));
}
void GeometryCore::TagMap::setMaxTag(EntityType type, int tag){
    this->_maxEntityTags.at(static_cast<int>(type)) = std::max(this->getMaxTag(type), tag);   
}
void GeometryCore::TagMap::incrementMaxTag(EntityType type){
    this->setMaxTag(type, getMaxTag(type) + 1);
}

void GeometryCore::TagMap::tagShape(const TopoDS_Vertex& shape){
    if (!_vertexTagMap.IsBound(shape)){
        this->incrementMaxTag(EntityType::Vertex);
        int tag = this->getMaxTag(EntityType::Vertex);
        // std::cout<<"Tagging vertex with tag: " << tag <<std::endl;
        this->_tagVertexMap.Bind(tag, shape);
        this->_vertexTagMap.Bind(shape, tag);
    }
}
void GeometryCore::TagMap::tagShape(const TopoDS_Edge& shape){
    if (!_edgeTagMap.IsBound(shape)){
        this->incrementMaxTag(EntityType::Edge);
        int tag = this->getMaxTag(EntityType::Edge);
        // std::cout<<"Tagging edge with tag: " << tag <<std::endl;
        this->_tagEdgeMap.Bind(tag, shape);
        this->_edgeTagMap.Bind(shape, tag);
    }
}
void GeometryCore::TagMap::tagShape(const TopoDS_Face& shape){
    if (!_faceTagMap.IsBound(shape)){
        this->incrementMaxTag(EntityType::Face);
        int tag = this->getMaxTag(EntityType::Face);
        // std::cout<<"Tagging face with tag: " << tag <<std::endl;
        this->_tagFaceMap.Bind(tag, shape);
        this->_faceTagMap.Bind(shape, tag);
    }
}
void GeometryCore::TagMap::tagShape(const TopoDS_Solid& shape){
    if (!_solidTagMap.IsBound(shape)){
        this->incrementMaxTag(EntityType::Solid);
        int tag = this->getMaxTag(EntityType::Solid);
        // std::cout<<"Tagging solid with tag: " << tag <<std::endl;
        this->_tagSolidMap.Bind(tag, shape);
        this->_solidTagMap.Bind(shape, tag);
    }
}
const int& GeometryCore::TagMap::getTag(const TopoDS_Vertex& shape){
    if (_vertexTagMap.IsBound(shape)){
        return this->_vertexTagMap.Find(shape);
    }else{
        std::cout<<"Vertex not bound!";
        return 0;
    }
}
const int& GeometryCore::TagMap::getTag(const TopoDS_Edge& shape){
    return this->_edgeTagMap.Find(shape);
}
const int& GeometryCore::TagMap::getTag(const TopoDS_Face& shape){
    return this->_faceTagMap.Find(shape);
}
const int& GeometryCore::TagMap::getTag(const TopoDS_Solid& shape){
    return this->_solidTagMap.Find(shape);
}

TopoDS_Shape GeometryCore::TagMap::getShape(EntityType type, int tag) {
    switch (type) {
        case EntityType::Vertex:
            if (_tagVertexMap.IsBound(tag)) {
                return _tagVertexMap.Find(tag);
            } else {
                throw "No vertex bound to this tag.";
            }
            break;
        case EntityType::Edge:
            if (_tagEdgeMap.IsBound(tag)) {
                return _tagEdgeMap.Find(tag);
            } else {
                throw "No edge bound to this tag.";
            }
            break;
        case EntityType::Face:
            if (_tagFaceMap.IsBound(tag)) {
                return _tagFaceMap.Find(tag);
            } else {
                throw "No face bound to this tag.";
            }
            break;
        case EntityType::Solid:
            if (_tagSolidMap.IsBound(tag)) {
                return _tagSolidMap.Find(tag);
            } else {
                throw "No solid bound to this tag.";
            }
            break;
        default:
            throw "Unknown entity type.";
    }
}

void GeometryCore::TagMap::tagEntities(const TopoDS_Shape& shape){
    
    TopExp_Explorer shapeExplorer;
    for(shapeExplorer.Init(shape, TopAbs_SOLID); shapeExplorer.More(); shapeExplorer.Next()){
        const TopoDS_Solid& solid = TopoDS::Solid(shapeExplorer.Current());
        this->tagShape(solid);
    }
    for(shapeExplorer.Init(shape, TopAbs_FACE); shapeExplorer.More(); shapeExplorer.Next()){
        const TopoDS_Face& face = TopoDS::Face(shapeExplorer.Current());
        this->tagShape(face);
    }
    for(shapeExplorer.Init(shape, TopAbs_EDGE); shapeExplorer.More(); shapeExplorer.Next()){
        const TopoDS_Edge& edge = TopoDS::Edge(shapeExplorer.Current());
        this->tagShape(edge);
    }
    for(shapeExplorer.Init(shape, TopAbs_VERTEX); shapeExplorer.More(); shapeExplorer.Next()){
        const TopoDS_Vertex& vertex = TopoDS::Vertex(shapeExplorer.Current());
        this->tagShape(vertex);
    }
}
