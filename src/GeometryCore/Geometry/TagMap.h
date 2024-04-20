#ifndef TAGMAP_H
#define TAGMAP_H

#include <stdexcept> 
#include <array>

#include <TopTools_DataMapOfShapeInteger.hxx>
#include <TopTools_DataMapOfIntegerShape.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>

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
            void tagShape(const TopoDS_Solid& shape);
            void tagShape(const TopoDS_Face& shape);
            void tagShape(const TopoDS_Edge& shape);
            void tagShape(const TopoDS_Vertex& shape);


            TopoDS_Shape getShape(EntityType type, int tag);

            const int& getTag(const TopoDS_Solid& shape);
            const int& getTag(const TopoDS_Face& shape);
            const int& getTag(const TopoDS_Edge& shape);
            const int& getTag(const TopoDS_Vertex& shape);

        private:
            std::array<int, static_cast<size_t>(EntityType::EntityTypeCount)> _maxEntityTags;

            int getMaxTag(EntityType type);
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

};
}
#endif