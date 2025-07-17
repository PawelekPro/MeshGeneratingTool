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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TDataStd_Name.hxx>
#include <TopLoc_Location.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include "StubShapes.hpp"
#include "OcafShapeDocumentImport.hpp"

class ShapeDocumentImportTest : public ::testing::Test{
    protected:
    std::shared_ptr<ShapeRegistry> shapeRegistry;
    ShapeSignalsPublisher publisher;
    Handle(TDocStd_Document) document;
    Handle(XCAFDoc_ShapeTool) shapeTool;
    Handle(XCAFDoc_ColorTool) colorTool;
    ShapeImportData cubeData;

    std::string name = "cubeShape";
    ColorRGBA color = ColorRGBA(0.5, 0.5, 0.5, 0.5);
    TDF_Label label;

    TopLoc_Location location;

    
    void SetUp() {
        auto app = XCAFApp_Application::GetApplication();
        app->NewDocument("XmlXCAF", document);
        app->InitDocument(document);

        shapeTool = XCAFDoc_DocumentTool::ShapeTool(document->Main());
        colorTool = XCAFDoc_DocumentTool::ColorTool(document->Main());

        gp_Trsf trsf;
        trsf.SetTranslation(gp_Vec(10.0, 20.0, 30.0));
        location = TopLoc_Location(trsf);
        auto shape = StubShapes::cube().Located(location);
        label = shapeTool->AddShape(shape);
        TDataStd_Name::Set(
            label, 
            TCollection_ExtendedString(
                name.c_str()
            )
        );
        colorTool->SetColor(label, color, XCAFDoc_ColorType::XCAFDoc_ColorGen);
    }

};


TEST_F(ShapeDocumentImportTest, ExtractShapeExtractsNameColorShapeLocation){
    ShapeImportData data = ShapeDocumentImport::extractShape(shapeTool, label);
    ASSERT_FALSE(data.shape.IsNull());    
    EXPECT_EQ(data.color, color);
    EXPECT_EQ(data.name, name);
    EXPECT_EQ(data.location, location);
}
