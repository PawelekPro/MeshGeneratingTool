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
        Solid = 3,
        NumEntityTypes = 4
    };

    using namespace std::string_literals;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class Geometry {
    public:
        const PartsMap& getShapesMap() const {return this->_shapesMap;};

        void importSTEP(const std::string& filePath, QWidget* progressBar);
        
        void importSTL(const std::string& filePath, QWidget* progressBar);
        
    private:
        PartsMap _shapesMap;
    };
};


#endif