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
#include "LabelKeyTool.hpp"
#include "ShapeIdFactory.hpp"

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
    Handle(XCAFDoc_ShapeTool) shapeTool;
    ShapeImportData cubeData;

    void SetUp() {
        attributeFactory = std::make_shared<AttributeFactory>(
           publisher 
        );

        auto app = XCAFApp_Application::GetApplication();
        app->NewDocument("XmlXCAF", document);
        app->InitDocument(document);

        shapeTool = XCAFDoc_DocumentTool::ShapeTool(document->Main());

        shapeRegistry = std::make_shared<OcafShapeRegistry>(
            document,
            attributeFactory
        );

        ColorRGBA color(0.5, 0.5, 0.5, 0.5);
        std::string name = "name";
        TopoDS_Shape cube = StubShapes::cube();
        TopLoc_Location loc;
        cubeData = ShapeImportData(
            cube, loc, name, color
        );

    }
};

TEST_F(OcafShapeRegistryTest, TestRegisteredShapeReturnsCorrectAttributes){
    auto shape = shapeRegistry->registerShape(cubeData);
    ASSERT_FALSE(shape->shape().IsNull());    
    EXPECT_EQ(shape->shape(), cubeData.shape);    
    EXPECT_EQ(shape->location(), cubeData.location);    
    EXPECT_EQ(shape->name(), cubeData.name);
    EXPECT_EQ(shape->color(), cubeData.color);    
}

TEST_F(OcafShapeRegistryTest, TestRegisteredComponentIsPlacedUnderMain){
    auto shape = shapeRegistry->registerShape(cubeData);
    ShapeId id = shape->id();
    auto label = LabelKeyTool::labelFromKey(
        document->Main(), 
        ShapeIdFactory::getKey(id)
    );
    auto foundLabel = shapeTool->FindShape(shape->shape());
    ASSERT_FALSE(foundLabel.IsNull());
    EXPECT_EQ(foundLabel, label);
}

TEST_F(OcafShapeRegistryTest, TestRegisteredComponentIsPlacedUnderParent){
    auto parent = shapeRegistry->registerShape(cubeData);
    ShapeId id = parent->id();
    auto parentLabel = LabelKeyTool::labelFromKey(
        document->Main(), 
        ShapeIdFactory::getKey(id)
    );
    
    auto shape = shapeRegistry->registerShape(cubeData, parentLabel);
    
    id = shape->id();
    auto childLabel = LabelKeyTool::labelFromKey(
        document->Main(), 
        ShapeIdFactory::getKey(id)
    );
    ASSERT_FALSE(parentLabel.IsNull());
    ASSERT_FALSE(childLabel.IsNull());
    EXPECT_TRUE(parent->isAssembly());
    EXPECT_EQ(childLabel.Father(), parentLabel);
}

TEST_F(OcafShapeRegistryTest, TestRegisterSameShapeUnderMainTwiceThrows) {
    auto shape = shapeRegistry->registerShape(cubeData);
    EXPECT_THROW(
        shapeRegistry->registerShape(cubeData),
        Exceptions::ShapeRegistry::ShapeAlreadyRegistered
    );
}

TEST_F(OcafShapeRegistryTest, TestRegisterSameShapeUnderParentTwiceAssignsDifferentIds) {
    auto parent = shapeRegistry->registerShape(cubeData);
    ShapeId id = parent->id();
    auto parentLabel = LabelKeyTool::labelFromKey(
        document->Main(), 
        ShapeIdFactory::getKey(id)
    );
    
    auto shape = shapeRegistry->registerShape(cubeData, parentLabel);
    id = shape->id();
    shape = shapeRegistry->registerShape(cubeData, parentLabel);
    EXPECT_NE(id, shape->id());
}