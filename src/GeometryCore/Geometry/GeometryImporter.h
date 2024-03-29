#ifndef GEOMETRYIMPORTER_H
#define GEOMETRYIMPORTER_H

#include <iostream>
#include <map>
#include <string>

#include <QWidget>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>

namespace GeometryCore {

    using namespace std::string_literals;
    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class GeometryImporter{
        public:
            virtual ~GeometryImporter() {}

        protected:
            virtual void import(const std::string& filename, QWidget* parent) = 0;
            ActorsMap getPartsActorMap();
            ActorsMap getFacesActorMap();
            ActorsMap getEdgesActorMap();
            std::string getUniqueObjectName(const std::string& prefix, const PartsMap& objectMap);

            PartsMap _partsMap;
            PartsMap _facesMap;
            PartsMap _edgesMap;
        private:

    };
}



#endif