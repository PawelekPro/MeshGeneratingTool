#ifndef GEOMETRYIMPORTER_H
#define GEOMETRYIMPORTER_H

#include <iostream>
#include <map>
#include <string>

#include <filesystem>

#include <QWidget>


#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkLogger.h>
#include <vtkProperty.h>

#include <BRep_Builder.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_DocumentTool.hxx>
namespace GeometryCore {

    using namespace std::string_literals;
    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class GeometryImporter{
        public:
            virtual ~GeometryImporter() {};
            ActorsMap getPartsActorMap();
            ActorsMap getFacesActorMap();
            ActorsMap getEdgesActorMap();

        protected:
            virtual void import(const std::string& filename, QWidget* parent) = 0;
            std::string getUniqueObjectName(const std::string& prefix, const PartsMap& objectMap);

            Handle(TDocStd_Document) _dataFrame;
            PartsMap _partsMap;
            PartsMap _facesMap;
            PartsMap _edgesMap;
        private:
            vtkSmartPointer<vtkActor> createVTKActor(TopoDS_Shape shape);
    };
}



#endif