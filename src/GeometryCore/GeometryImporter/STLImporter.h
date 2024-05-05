#ifndef STLImporter_H
#define STLImporter_H

#include "GeometryImporter.h"
#include "ProgressBar.h"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <RWStl.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>


namespace GeometryCore {
    class STLImporter : public GeometryImporter{
        public:
            void import(const std::string& filename, QWidget* parent) override;
            const ActorsMap getPartsActorMap() override;
        private:
    };
}
#endif