#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GeometryFunctions.h"
#include "GeometryImporter.h"
#include "STEPImporter.h"

namespace GeometryCore {
    using namespace std::string_literals;
    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class Geometry {
    public:
        PartsMap getPartsMap(){return this->_partsMap;};
        PartsMap getFacesMap(){return this->_facesMap;};
        PartsMap getEdgesMap(){return this->_edgesMap;};
        ActorsMap getPartsActorMap(){return this->_partsActorMap;};
        ActorsMap getFacesActorMap(){return this->_facesActorMap;};
        ActorsMap getEdgesActorMap(){return this->_edgesActorMap;};

        void importSTEP(const std::string& filePath);
        void importSTL(const std::string& filePath);
        
    private:
        PartsMap _partsMap;
        PartsMap _facesMap;
        PartsMap _edgesMap;

        ActorsMap _partsActorMap;
        ActorsMap _facesActorMap;
        ActorsMap _edgesActorMap;
    };
};  


#endif