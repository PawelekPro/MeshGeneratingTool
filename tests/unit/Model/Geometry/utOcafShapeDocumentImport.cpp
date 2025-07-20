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

#include "StubShapeDocument.hpp"
#include "OcafShapeDocumentImport.hpp"
#include "ShapeIdFactory.hpp"

class StubShape : public Shape {
    virtual ShapeId id() const override {
        return ShapeIdFactory::create(
            std::make_shared<ShapeKey>(std::vector<int>{1,2,3})
        );
    };

    virtual bool isAssembly() const override {return true;}
    virtual TopoDS_Shape shape() const override {return TopoDS_Shape();}
    virtual TopLoc_Location location() const override {return TopLoc_Location();}
    
    virtual std::string name() const override {return "name";}
    virtual ColorRGBA color() const override {return ColorRGBA();}
};

class MockShapeRegistry : public ShapeRegistry {
public:
    MOCK_METHOD(std::shared_ptr<Shape>, registerShape,
                (const ShapeImportData& aShapeData, TDF_Label aLocalParent), (override));

    MOCK_METHOD(TDF_Label, baseLabel, (), (override));
};

MATCHER_P(ShapeImportDataEq, expected, "Matches ShapeImportData fields") {
    return  arg.name == expected.name &&
            arg.color == expected.color &&
            arg.location == expected.location &&
            arg.shape == expected.shape;
};

class ShapeDocumentImportTest : public ::testing::Test{
    protected:
    StubShapeDocument stubDoc;
    std::shared_ptr<MockShapeRegistry> mockRegistry;
    std::shared_ptr<OcafShapeDocumentImporter> importer;

    void SetUp() override {
        stubDoc = StubShapeDocument();
        mockRegistry = std::make_shared<MockShapeRegistry>();
        ON_CALL(*mockRegistry, registerShape)
            .WillByDefault(testing::Return(std::make_shared<StubShape>()));

        ON_CALL(*mockRegistry, baseLabel)
            .WillByDefault(testing::Return(TDF_Label{}));

        importer = std::make_shared<OcafShapeDocumentImporter>(
            mockRegistry
        );
    }
};

TEST_F(ShapeDocumentImportTest, ImportDocumentWithOneShapeCallsRegistryWithCorrectData) {
    stubDoc.addFree();
    ShapeImportData expectedData(
        stubDoc.freeShape, 
        stubDoc.freeLocation, 
        stubDoc.freeName, 
        stubDoc.freeColor
    );
    EXPECT_CALL(*mockRegistry, baseLabel())
        .WillRepeatedly(testing::Return(TDF_Label{}));

    EXPECT_CALL(*mockRegistry, registerShape(
        ShapeImportDataEq(expectedData),
        TDF_Label{}
    )).Times(1);

    importer->importDocument(stubDoc.document);

}



// TEST_F(ShapeDocumentImportTest, ImportPartCallsRegistryWithCorrectShapeData) {
//     ShapeImportData expectedData = ShapeDocumentImport::extractShape(
//         stubDoc.shapeTool, stubDoc.freeLabel
//     );

//     EXPECT_CALL(*mockRegistry, baseLabel())
//         .WillOnce(testing::Return(TDF_Label{}));

//     EXPECT_CALL(*mockRegistry, registerShape(
//         ShapeImportDataEq(expectedData),
//         TDF_Label{}
//     )).Times(1);

//     ShapeDocumentImport::importPart(
//         stubDoc.shapeTool, stubDoc.freeLabel,
//         mockRegistry, stubDoc.freeLabel
//     );
// }


// TEST_F(ShapeDocumentImportTest, ImportAssemblyCallsRegistryWithCorrectShapeData){
//     ShapeDocumentImport::importAssembly(
//         stubDoc.shapeTool, stubDoc.freeLabel, 
//         mockRegistry, mockRegistry->baseLabel()
//     );

// }

// TEST_F(ShapeDocumentImportTest, ImportDocumentCallsRegistryForAllShapes){
//     bool importer = ShapeDocumentImport::importDocument(
//         mockRegistry, stubDoc.document
//     );

// }