#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>
#include <array>

#include "STEPImporter.h"
#include "STLImporter.h"
#include "TagMap.h"

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


        std::string getShapeName(const TopoDS_Shape& shape);

        std::vector<std::string> getShapesNames(std::vector<std::reference_wrapper<const TopoDS_Shape>> shapesVec);

    private:
        PartsMap _shapesMap;
        TagMap _tagMap;
    };

    // Geometry utils 
};


#endif