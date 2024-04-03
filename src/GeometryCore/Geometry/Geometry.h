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
        const PartsMap& getPartsMap() const {return this->_partsMap;};
        const PartsMap& getFacesMap() const {return this->_facesMap;};
        const PartsMap& getEdgesMap() const {return this->_edgesMap;};
        const ActorsMap& getPartsActorMap() const {return this->_partsActorMap;};
        const ActorsMap& getFacesActorMap() const {return this->_facesActorMap;};
        const ActorsMap& getEdgesActorMap() const {return this->_edgesActorMap;};

        void importSTEP(const std::string& filePath, QWidget* progressBar);
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