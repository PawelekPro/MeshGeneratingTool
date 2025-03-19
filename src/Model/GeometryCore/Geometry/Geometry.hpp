#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <string>
#include <vector>
#include <array>

#include "TagMap.hpp"
#include "ModelSubject.hpp"
#include "CommandStack.hpp"
#include "GeoService.hpp"
#include "GeoCommandsFactory.hpp"
namespace GeometryCore {

    using namespace std::string_literals;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class Geometry {
    public:
        Geometry(const ModelSubject&, CommandStack&);
        ~Geometry();

        void importSTEP(const std::string& filePath);
        
        void importSTL(const std::string& filePath);
        
        const TagMap& getTagMap() const {return this->_tagMap;};
        
        PartsMap getShapes() const;

        std::vector<int> getShapeVerticesTags(const TopoDS_Shape& shape);

        std::vector<int> getShapesVerticesTags(std::vector<std::reference_wrapper<const TopoDS_Shape>> shapesVec);

        std::string getShapeName(const TopoDS_Shape& shape);

        std::vector<std::string> getShapesNames(std::vector<std::reference_wrapper<const TopoDS_Shape>> shapesVec);

    private:
        
        GeoState _geoState;
        GeoService _geoService;
        GeoCommandsFactory _commandFactory;
        TagMap _tagMap;

        const ModelSubject& _subject;
        CommandStack& _commandStack; 
    };

    // Geometry utils 
};


#endif