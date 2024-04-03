#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "STEPImporter.h"
#include "STLImporter.h"
#include "GeometryImporter.h"


#include "ProgressBar.h"
#include "ProgressBarPlugin.hpp"

#include <iostream>
#include <map>
#include <string>
#include <filesystem>

#include <QApplication>
#include <QObject>
#include <QProgressDialog>
#include <QWidget>

#include <vtkActor.h>
#include <vtkLogger.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>

#include <BRep_Builder.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
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

    class Geometry {
    public:
        const PartsMap& getPartsMap() const {return this->_partsMap;};
        const PartsMap& getFacesMap() const {return this->_facesMap;};
        const PartsMap& getEdgesMap() const {return this->_edgesMap;};
        const ActorsMap& getPartsActorMap() const {return this->_partsActorMap;};
        const ActorsMap& getFacesActorMap() const {return this->_facesActorMap;};
        const ActorsMap& getEdgesActorMap() const {return this->_edgesActorMap;};

        void importSTEP(const std::string& filePath, QWidget* progressBar);
        void importSTL(const std::string& filePath, QWidget* progressBar);
        
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