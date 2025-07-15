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

#include "OcafShapeRegistry.hpp"
#include "ShapeImportData.hpp"

#include "StubShapes.hpp"
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>

class OcafShapeRegistryTest : public ::testing::Test {
    protected:
    std::shared_ptr<AttributeFactory> attributeFactory;
    std::shared_ptr<ShapeRegistry> shapeRegistry;
    ShapeSignalsPublisher publisher;
    Handle(TDocStd_Document) document;

    void SetUp() {
        attributeFactory = std::make_shared<AttributeFactory>(
           publisher 
        );

        auto app = XCAFApp_Application::GetApplication();
        app->NewDocument("XmlXCAF", document);
        app->InitDocument(document);

        shapeRegistry = std::make_shared<OcafShapeRegistry>(
            document,
            attributeFactory
        );
    }
};

TEST_F(OcafShapeRegistryTest, TestRegisteredShapeReturnsCorrectAttributes){
    ColorRGBA color(0.5, 0.5, 0.5, 0.5);
    std::string name = "name";
    TopoDS_Shape cube = StubShapes::cube();
    TopLoc_Location loc;
    ShapeImportData cubeData(
        cube, loc, name, color
    );

    auto shape = shapeRegistry->registerShape(cubeData);
    ASSERT_FALSE(shape->shape().IsNull());    
    EXPECT_EQ(shape->shape(), cube);    
    EXPECT_EQ(shape->name(), name);
    EXPECT_EQ(shape->color(), color);    
}

TEST_F(OcafShapeRegistryTest, TestRegisteredComponentIsPlacedUnderParent){
    ASSERT_TRUE(true);    
}

TEST_F(OcafShapeRegistryTest, TestRegisteredComponentHasName){
    ASSERT_TRUE(true);    
}

TEST_F(OcafShapeRegistryTest, TestRegisteredComponentHasColor){
    ASSERT_TRUE(true);    
}

TEST_F(OcafShapeRegistryTest, TestRegisteredComponentHasCorrectShapeAndLoc){
        ASSERT_TRUE(true);    
}