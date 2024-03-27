#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GeometryFunctions.h"

namespace GeometryCore {
    using namespace std::string_literals;
    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using PartsMap = std::map<std::string, TopoDS_Shape>;



    class Geometry {
    public:
        void getPartsMap();

    private:
        PartsMap partsMap;
        PartsMap facesMap;
        PartsMap edgesMap;

        ActorsMap partsActorMap;
        ActorsMap facesActorMap;
        ActorsMap edgesActorMap;
    };
};  


#endif