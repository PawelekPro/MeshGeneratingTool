#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <array>
#include "STEPImporter.h"
#include "STLImporter.h"
#include "GeometryImporter.h"
#include <TopTools_DataMapOfShapeInteger.hxx>
#include <TopTools_DataMapOfIntegerShape.hxx>

namespace GeometryCore {
    enum class EntityType : int {
        Vertex = 0,
        Line = 1,
        Face = 2,
        Volume = 3,
        NumEntityTypes = 4
    };

    using namespace std::string_literals;
    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class Geometry {
    public:
        const PartsMap& getPartsMap() const {return this->_partsMap;};
        const PartsMap& getFacesMap() const {return this->_facesMap;};
        const PartsMap& getEdgesMap() const {return this->_edgesMap;};
        const ActorsMap& getPartsActorMap() const {return this->_partsActorMap;};
        const ActorsMap& getFacesActorMap() const {return this->_facesActorMap;};
        const ActorsMap& getEdgesActorMap() const {return this->_edgesActorMap;};

        void importSTEP(const std::string& filePath, QWidget* progressBar);
        void importSTL(const std::string& filePath, QWidget* progressBar);

        void addShape(const TopoDS_Shape& shape, int dim);
        
    private:
        std::array<int, static_cast<size_t>(EntityType::NumEntityTypes)> maxEntityTags;

        int getMaxTag(EntityType dim);
        void setMaxTag(EntityType dim, int tag);
        void incrementMaxTag(EntityType dim);

        PartsMap _partsMap;
        PartsMap _facesMap;
        PartsMap _edgesMap;

        ActorsMap _partsActorMap;
        ActorsMap _facesActorMap;
        ActorsMap _edgesActorMap;

        TopTools_DataMapOfIntegerShape _vertexTags;
        TopTools_DataMapOfIntegerShape _edgeTags;
        TopTools_DataMapOfIntegerShape _faceTags;
        TopTools_DataMapOfIntegerShape _volumeTags;
    };
};


#endif