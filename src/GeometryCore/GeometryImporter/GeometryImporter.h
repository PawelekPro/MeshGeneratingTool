#ifndef GEOMETRYIMPORTER_H
#define GEOMETRYIMPORTER_H

#include <iostream>
#include <map>
#include <string>
#include <filesystem>

#include <QWidget>

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>

#include <TopoDS_Shape.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <Message_ProgressIndicator.hxx>
#include <TDocStd_Document.hxx>

namespace GeometryCore {

    using namespace std::string_literals;
    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using PartsMap = std::map<std::string, TopoDS_Shape>;

    class GeometryImporter{
        public:
            virtual ~GeometryImporter() {};
            virtual const ActorsMap getPartsActorMap();
            virtual const ActorsMap getFacesActorMap();
            virtual const ActorsMap getEdgesActorMap();
            const PartsMap getPartsMap(){return this->_partsMap;};
            const PartsMap getFacesMap(){return this->_facesMap;};
            const PartsMap getEdgesMap(){return this->_edgesMap;};

        protected:
            virtual void import(const std::string& filename, QWidget* parent) = 0;
            std::string getUniqueObjectName(const std::string& prefix, const PartsMap& objectMap);
            vtkSmartPointer<vtkActor> createVTKActor(const TopoDS_Shape& shape);

            Handle(TDocStd_Document) _dataFrame;
            PartsMap _partsMap;
            PartsMap _facesMap;
            PartsMap _edgesMap;
        private:

    };
}

#endif