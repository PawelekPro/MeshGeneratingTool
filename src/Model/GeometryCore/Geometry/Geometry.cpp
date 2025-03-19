#include "Geometry.hpp"
#include "ShapeEvents.hpp"
#include "ImportSTEPCommand.hpp"


GeometryCore::Geometry::Geometry(
    const ModelSubject& aModelSubject, 
    CommandStack& aCommandStack
    ) : 
    _subject(aModelSubject),
    _commandStack(aCommandStack),
    _geoState(),
    _geoService(aModelSubject, _geoState),
    _commandFactory(aModelSubject, _geoService){}

GeometryCore::Geometry::~Geometry(){};

void GeometryCore::Geometry::importSTEP(const std::string& aFilePath){
    std::unique_ptr<ImportSTEPCommand> command =
        _commandFactory.importSTEP(aFilePath);
    _commandStack.execute(std::move(command));
};

GeometryCore::PartsMap GeometryCore::Geometry::getShapes() const {
    return _geoState.shapes();
}


void GeometryCore::Geometry::importSTL(const std::string& filePath){
    return;
};

std::vector<int> GeometryCore::Geometry::getShapeVerticesTags(const TopoDS_Shape& shape){
    std::vector<int> vertexTags;
    // int vertex_tag;
    // TopExp_Explorer shapeExplorer(shape, TopAbs_VERTEX);
    // for(; shapeExplorer.More(); shapeExplorer.Next()) {
    //     const TopoDS_Vertex& vertex = TopoDS::Vertex(shapeExplorer.Current());
    //     vertex_tag = _tagMap.getTag(vertex);
    //     vertexTags.push_back(vertex_tag);
    // }
    // std::sort(vertexTags.begin(), vertexTags.end());
    // vertexTags.erase(std::unique(vertexTags.begin(), vertexTags.end()), vertexTags.end());
    return vertexTags;
}

std::vector<int> GeometryCore::Geometry::getShapesVerticesTags(std::vector<std::reference_wrapper<const TopoDS_Shape>> shapesVec) {
    std::vector<int> verticesTags;
    // for (const auto& shape : shapesVec) {
    //     std::vector<int> shapeVerticesTags = getShapeVerticesTags(shape);
    //     verticesTags.insert(verticesTags.end(), shapeVerticesTags.begin(), shapeVerticesTags.end());
    // }
    // std::sort(verticesTags.begin(), verticesTags.end());
    // verticesTags.erase(std::unique(verticesTags.begin(), verticesTags.end()), verticesTags.end());

    return verticesTags;
}

std::vector<std::string> GeometryCore::Geometry::getShapesNames(std::vector<std::reference_wrapper<const TopoDS_Shape>> shapesVec){
    std::vector<std::string> shapesNames;
    for(const TopoDS_Shape& shapeRef : shapesVec){
        shapesNames.push_back("shape");
    }
    return shapesNames;
}

std::string GeometryCore::Geometry::getShapeName(const TopoDS_Shape& shape){
    std::string name = "shape";
    return name;
}
