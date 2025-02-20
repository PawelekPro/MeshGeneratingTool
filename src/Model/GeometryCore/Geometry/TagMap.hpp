#ifndef TAGMAP_HPP
#define TAGMAP_HPP

#include <stdexcept> 
#include <array>
#include <map>

#include <TopTools_DataMapOfShapeInteger.hxx>
#include <TopTools_DataMapOfIntegerShape.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>

namespace GeometryCore {

    enum class EntityType : int {
    Vertex = 0,
    Edge = 1,
    Face = 2,
    Solid = 3,
    EntityTypeCount = 4
    };

    class TagMap{

        public:

            TagMap();

            void tagEntities(const TopoDS_Shape& shape);
            
            void tagShape(const TopoDS_Solid& shape);
            void tagShape(const TopoDS_Face& shape);
            void tagShape(const TopoDS_Edge& shape);
            void tagShape(const TopoDS_Vertex& shape);

            TopoDS_Shape getShape(EntityType type, int tag);
            const TopoDS_Shape& getShape(EntityType type, int tag) const;

            int getTag(const TopoDS_Shape& shape) const;
            int getTag(const TopoDS_Solid& shape) const;
            int getTag(const TopoDS_Face& shape) const;
            int getTag(const TopoDS_Edge& shape) const;
            int getTag(const TopoDS_Vertex& shape) const;

        private:
            std::array<int, static_cast<size_t>(EntityType::EntityTypeCount)> _maxEntityTags;

            int getMaxTag(EntityType type) const;
            void setMaxTag(EntityType type, int tag);
            void incrementMaxTag(EntityType type);

            TopTools_DataMapOfIntegerShape _tagVertexMap;
            TopTools_DataMapOfIntegerShape _tagEdgeMap;
            TopTools_DataMapOfIntegerShape _tagFaceMap;
            TopTools_DataMapOfIntegerShape _tagSolidMap;
            
            TopTools_DataMapOfShapeInteger _vertexTagMap;
            TopTools_DataMapOfShapeInteger _edgeTagMap;
            TopTools_DataMapOfShapeInteger _faceTagMap;
            TopTools_DataMapOfShapeInteger _solidTagMap;

            std::map<std::string, int> _nameTagMap;
            std::map<int, std::string> _tagNameMap;

};
}
#endif