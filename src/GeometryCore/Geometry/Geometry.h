#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <array>
#include "STEPImporter.h"
#include "STLImporter.h"
#include "GeometryImporter.h"
#include "TagMap.h"
#include <TopTools_DataMapOfShapeInteger.hxx>
#include <TopTools_DataMapOfIntegerShape.hxx>


namespace GeometryCore {

    using namespace std::string_literals;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class Geometry {
    public:
        Geometry();
        ~Geometry();
        const PartsMap& getShapesMap() const {return this->_shapesMap;};

        void importSTEP(const std::string& filePath, QWidget* progressBar);
        
        void importSTL(const std::string& filePath, QWidget* progressBar);

        std::vector<int> getShapeVerticesTags(const TopoDS_Shape& shape);

        
    private:
        PartsMap _shapesMap;
        TagMap _tagMap;
    };

    // Geometry utils 
};


#endif