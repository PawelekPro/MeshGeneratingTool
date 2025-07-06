#ifndef MOCKCUBEASSEMBLYIMPORTER_HPP
#define MOCKCUBEASSEMBLYIMPORTER_HPP 

#include "ShapeImporter.hpp"
#include "StubShapes.hpp"
#include <BRepBuilderAPI_Transform.hxx>

#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_Label.hxx>

class MockCubesAssemblyImporter : public ShapeImporter {
public:
    Handle(TDocStd_Document) import(
        const std::string& aFilePath,
        const ProgressIndicator& aProgressIndicator = IdleProgressIndicator()
    ) const override 
    {
        aProgressIndicator.progress("start", 0);
        Handle(TDocStd_Document) doc = initDocument();
        auto shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

        TDF_Label assemblyLabel = shapeTool->NewShape();

        TopoDS_Shape cube1 = StubShapes::cube();

        aProgressIndicator.progress("progress", 50);
        TDF_Label cube1Label = shapeTool->AddComponent(assemblyLabel, cube1);

        TopoDS_Shape cube2 = StubShapes::cube();
        gp_Trsf translate;
        translate.SetTranslation(gp_Vec(5.0, 0.0, 0.0));

        BRepBuilderAPI_Transform transformer(cube2, translate);
        TopoDS_Shape shiftedCube2 = transformer.Shape();

        TDF_Label cube2Label = shapeTool->AddComponent(
            assemblyLabel, shiftedCube2
        );

        aProgressIndicator.progress("finish", 100);
        return doc;
    }
};

#endif