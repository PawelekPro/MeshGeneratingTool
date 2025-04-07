#include "TagMap.hpp"

GeometryCore::TagMap::TagMap() {
    std::fill(_maxEntityTags.begin(), _maxEntityTags.end(), 0);
}

int GeometryCore::TagMap::getMaxTag(EntityType type) const {
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

int GeometryCore::TagMap::getTag(const TopoDS_Shape& aShape) const {
    const TopAbs_ShapeEnum shapeType = aShape.ShapeType();
    switch (shapeType)
    {
        case TopAbs_VERTEX:
            return getTag(static_cast<const TopoDS_Vertex&>(aShape));
        
        case TopAbs_EDGE:
            return getTag(static_cast<const TopoDS_Edge&>(aShape));
        
        case TopAbs_FACE:
            return getTag(static_cast<const TopoDS_Face&>(aShape));
        
        case TopAbs_SOLID:
            return getTag(static_cast<const TopoDS_Solid&>(aShape));
        
        default:
            std::cerr << "Unsupported shape type!" << std::endl;
            return 0;
    }
}

int GeometryCore::TagMap::getTag(const TopoDS_Vertex& shape) const {
    if (_vertexTagMap.IsBound(shape)) {
        return _vertexTagMap.Find(shape);
    } else {
        std::cerr << "Vertex not bound!" << std::endl;
        return 0;
    }
}

int GeometryCore::TagMap::getTag(const TopoDS_Edge& shape) const {
    if (_edgeTagMap.IsBound(shape)) {
        return _edgeTagMap.Find(shape);
    } else {
        std::cerr << "Edge not bound!" << std::endl;
        return 0;
    }
}

int GeometryCore::TagMap::getTag(const TopoDS_Face& shape) const {
    if (_faceTagMap.IsBound(shape)) {
        return _faceTagMap.Find(shape);
    } else {
        std::cerr << "Face not bound!" << std::endl;
        return 0;
    }
}

int GeometryCore::TagMap::getTag(const TopoDS_Solid& shape) const {
    if (_solidTagMap.IsBound(shape)) {
        return _solidTagMap.Find(shape);
    } else {
        std::cerr << "Solid not bound!" << std::endl;
        return 0;
    }
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


const TopoDS_Shape& GeometryCore::TagMap::getShape(EntityType type, int tag) const {
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
