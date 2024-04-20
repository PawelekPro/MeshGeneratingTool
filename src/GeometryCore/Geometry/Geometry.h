#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <array>
#include "STEPImporter.h"
#include "STLImporter.h"
#include "GeometryImporter.h"
#include "TagMap.h"


#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>

namespace GeometryCore {
    using namespace std::string_literals;
    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class Geometry {
    public:
        const PartsMap& getSolidsMap() const {return this->_solidsMap;};
        const PartsMap& getFacesMap() const {return this->_facesMap;};
        const PartsMap& getEdgesMap() const {return this->_edgesMap;};
        const ActorsMap& getPartsActorMap() const {return this->_partsActorMap;};
        const ActorsMap& getFacesActorMap() const {return this->_facesActorMap;};
        const ActorsMap& getEdgesActorMap() const {return this->_edgesActorMap;};

        void importSTEP(const std::string& filePath, QWidget* progressBar);
        void importSTL(const std::string& filePath, QWidget* progressBar);

        
    private:
        TagMap _tagMap;
        
        PartsMap _solidsMap;
        PartsMap _facesMap;
        PartsMap _edgesMap;
        PartsMap _vertexMap;

        ActorsMap _partsActorMap;
        ActorsMap _facesActorMap;
        ActorsMap _edgesActorMap;
    };
};


#endif