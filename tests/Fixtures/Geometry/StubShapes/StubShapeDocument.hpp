/*
* Copyright (C) 2024 Krystian Fudali
*
* This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include "Color.hpp"
#include "StubShapes.hpp"

class StubShapeDocument {
    public:
    StubShapeDocument(){
        auto app = XCAFApp_Application::GetApplication();
        app->NewDocument("XmlXCAF", document);
        app->InitDocument(document);

        shapeTool = XCAFDoc_DocumentTool::ShapeTool(document->Main());
        colorTool = XCAFDoc_DocumentTool::ColorTool(document->Main());

        gp_Trsf trsf;
        assemblyLabel = shapeTool->NewShape();
        setName(assemblyLabel, assemblyName);

        trsf.SetTranslation(gp_Vec(1.0, 2.0, 3.0));
        childSphereLocation = TopLoc_Location(trsf);
        childSphereShape = StubShapes::cube();
        childSphereLabel = shapeTool->AddComponent(
            assemblyLabel,
            childSphereShape.Located(childSphereLocation)
        );

        setName(childSphereLabel, childSphereName);
        colorTool->SetColor(childSphereLabel, childSphereColor, XCAFDoc_ColorType::XCAFDoc_ColorGen);

        trsf.SetTranslation(gp_Vec(4.0, 5.0, 6.0));
        childCubeLocation = TopLoc_Location(trsf);
        childCubeShape = StubShapes::cube();
        childCubeLabel = shapeTool->AddComponent(
            assemblyLabel, childCubeShape.Located(childCubeLocation)
        );
        setName(childCubeLabel, childCubeName);
        colorTool->SetColor(childCubeLabel, childCubeColor, XCAFDoc_ColorType::XCAFDoc_ColorGen);       

        trsf.SetTranslation(gp_Vec(7.0, 8.0, 9.0));
        freeLocation = TopLoc_Location(trsf);
        freeShape = StubShapes::sphere().Located(freeLocation);
        freeLabel = shapeTool->AddShape(freeShape);
        setName(freeLabel, freeName);
        colorTool->SetColor(freeLabel, freeColor, XCAFDoc_ColorType::XCAFDoc_ColorGen);
    }

    Handle(TDocStd_Document) document;
    Handle(XCAFDoc_ShapeTool) shapeTool;
    Handle(XCAFDoc_ColorTool) colorTool;
    
    TDF_Label assemblyLabel;
    std::string assemblyName = "assemblyName";

    TopoDS_Shape childSphereShape;
    std::string childSphereName = "childSphereName";
    ColorRGBA childSphereColor = ColorRGBA{0.1, 0.1, 0.1, 0.1};
    TDF_Label childSphereLabel;
    TopLoc_Location childSphereLocation;

    TopoDS_Shape childCubeShape;
    std::string childCubeName = "childCubeName";
    ColorRGBA childCubeColor = ColorRGBA{0.2, 0.2, 0.2, 0.2};
    TDF_Label childCubeLabel;
    TopLoc_Location childCubeLocation;

    TopoDS_Shape freeShape;
    std::string freeName = "freeName";
    ColorRGBA freeColor = ColorRGBA{0.3, 0.3, 0.3, 0.3};
    TDF_Label freeLabel;
    TopLoc_Location freeLocation;

    private:
    void setName(TDF_Label label, const std::string& name){
        TDataStd_Name::Set(
            label, 
            TCollection_ExtendedString(
                name.c_str()
            )
        );
    }
};