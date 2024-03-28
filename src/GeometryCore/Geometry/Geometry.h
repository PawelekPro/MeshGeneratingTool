#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GeometryFunctions.h"

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

        PartsMap setPartsMap(const PartsMap& partsMap){this->_partsMap = std::move(partsMap);};
        PartsMap setFacesMap(const PartsMap& facesMap){this->_partsMap = std::move(facesMap);};
        PartsMap setEdgesMap(const PartsMap& edgesMap){this->_partsMap = std::move(edgesMap);};
        
        ActorsMap setPartsActorMap(const ActorsMap& partsMap){this->_partsActorMap = std::move(partsMap);};
        ActorsMap setFacesActorMap(const ActorsMap& facesMap){this->_facesActorMap = std::move(facesMap);};
        ActorsMap setEdgesActorMap(const ActorsMap& edgesMap){this->_edgesActorMap = std::move(edgesMap);};

        void importStep(std::string filePath);
        void importSTL(std::string filePath);
        
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