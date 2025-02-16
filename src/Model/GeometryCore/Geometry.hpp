#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <string>
#include <vector>
#include <array>

#include "STEPImporter.hpp"
#include "STLImporter.hpp"
#include "TagMap.hpp"
#include "ModelSubject.hpp"
#include "OcafDoc.hpp"
namespace GeometryCore {

    using namespace std::string_literals;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class Geometry {
    public:
        Geometry(const ModelSubject& aModelSubject) : _subject(aModelSubject), _doc(aModelSubject){};
        ~Geometry();

        const OcafDoc& doc() const {return _doc;};

        const PartsMap& getShapesMap() const {return this->_shapesMap;};

        const TagMap& getTagMap() const {return this->_tagMap;};

        void importSTEP(const std::string& filePath);
        
        void importSTL(const std::string& filePath);

        std::vector<int> getShapeVerticesTags(const TopoDS_Shape& shape);

        std::vector<int> getShapesVerticesTags(std::vector<std::reference_wrapper<const TopoDS_Shape>> shapesVec);

        std::string getShapeName(const TopoDS_Shape& shape);

        std::vector<std::string> getShapesNames(std::vector<std::reference_wrapper<const TopoDS_Shape>> shapesVec);

    private:

        
        
        const ModelSubject& _subject;
        PartsMap _shapesMap;
        TagMap _tagMap;
        OcafDoc _doc;
    };

    // Geometry utils 
};


#endif