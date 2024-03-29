#ifndef STEPImporter_H
#define STEPImporter_H

#include <filesystem>

#include <QWidget>

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

#include "GeometryImporter.h"
#include "ProgressBarPlugin.hpp"

namespace GeometryCore {
    class STEPImporter : public GeometryImporter{
        public:
            void import(const std::string& filename, QWidget* parent) override;
            void actorMaps();
        private:
        	Handle(TDocStd_Document) _dataFrame;
    };
}
#endif