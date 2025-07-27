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
#include "OcafShapeRegistry.hpp"
#include "AttributeFactory.hpp"


class SpyShapeRegistry : public OcafShapeRegistry {
public:
    using OcafShapeRegistry::OcafShapeRegistry;

    MOCK_METHOD(std::shared_ptr<Shape>, registerShape,
                (const ShapeImportData& aShapeData, TDF_Label aLocalParent), (override));

    std::shared_ptr<Shape> realRegisterShape(const ShapeImportData& aShapeData, TDF_Label aLocalParent) {
        return OcafShapeRegistry::registerShape(aShapeData, aLocalParent);
    }
};

MATCHER_P(ShapeImportDataEq, expected, "Matches ShapeImportData fields") {
    bool nameEqual = arg.name == expected.name;
    bool colorEqual = arg.color == expected.color;
    bool locationEqual = arg.location == expected.location;
    bool shapeEqual = arg.shape.TShape() == expected.shape.TShape();
    return nameEqual && colorEqual && locationEqual && shapeEqual;
};

class RegistryDocumentImporterIntegrationTest : public ::testing::Test{
    protected:
    StubShapeDocument stubSourceDoc;
    StubShapeDocument stubDestDoc;
    ShapeSignalsPublisher publisher;
    std::shared_ptr<AttributeFactory> attrFactory;
    std::shared_ptr<ShapeRegistry> registry;
    std::shared_ptr<OcafShapeDocumentImporter> importer;

    void SetUp() override {
        stubSourceDoc = StubShapeDocument();
        stubDestDoc = StubShapeDocument();

        attrFactory = std::make_shared<AttributeFactory>(publisher);
        registry = std::make_shared<SpyShapeRegistry>(
            stubDestDoc.document, attrFactory
        );
        importer = std::make_shared<OcafShapeDocumentImporter>(
            registry
        );
    }
};

TEST_F(RegistryDocumentImporterIntegrationTest, RegistryGetsCalledOnceOnDocumentWithSingleFileImport) {
    EXPECT_CALL(*std::static_pointer_cast<SpyShapeRegistry>(registry),
            registerShape(::testing::_, ::testing::_))
    .WillOnce([&](const ShapeImportData& data, TDF_Label label) {
        return std::static_pointer_cast<SpyShapeRegistry>(registry)->realRegisterShape(
            data, label
        );
    });

    stubSourceDoc.addFree();
    importer->importDocument(stubSourceDoc.document);
}

TEST_F(RegistryDocumentImporterIntegrationTest, RegistryGetsCalledForAssemblyAndAllPartsInside) {
    stubSourceDoc.addAssembly();

    ShapeImportData expectedAssembly(
        stubSourceDoc.assemblyShape,
        stubSourceDoc.assemblyLocation,
        stubSourceDoc.assemblyName,
        stubSourceDoc.assemblyColor
    );
    ShapeImportData expectedCube(
        stubSourceDoc.childCubeShape,
        stubSourceDoc.childCubeLocation,
        stubSourceDoc.childCubeName,
        stubSourceDoc.childCubeColor
    );
    ShapeImportData expectedSphere(
        stubSourceDoc.childSphereShape,
        stubSourceDoc.childSphereLocation,
        stubSourceDoc.childSphereName,
        stubSourceDoc.childSphereColor
    );

    std::vector<ShapeImportData> actualCalls;

    EXPECT_CALL(*std::static_pointer_cast<SpyShapeRegistry>(registry),
                registerShape(::testing::_, ::testing::_))
        .Times(3)
        .WillRepeatedly([&](const ShapeImportData& data, TDF_Label label) {
            actualCalls.push_back(data);
            return std::static_pointer_cast<SpyShapeRegistry>(
                registry
            )->realRegisterShape(data, label);
        });


    importer->importDocument(stubSourceDoc.document);
    EXPECT_THAT(actualCalls, ::testing::UnorderedElementsAre(
        ShapeImportDataEq(expectedAssembly),
        ShapeImportDataEq(expectedCube),
        ShapeImportDataEq(expectedSphere)
    ));
}

TEST_F(RegistryDocumentImporterIntegrationTest, DestDocAfterImportHasCorrectPartCount) {
    stubSourceDoc.addAssembly();
    stubSourceDoc.addFree();

    EXPECT_CALL(*std::static_pointer_cast<SpyShapeRegistry>(registry),
                registerShape(::testing::_, ::testing::_))
        .Times(4)
        .WillRepeatedly([&](const ShapeImportData& data, TDF_Label label) {
            return std::static_pointer_cast<SpyShapeRegistry>(
                registry
            )->realRegisterShape(data, label);
        });
    importer->importDocument(stubSourceDoc.document);

    TDF_LabelSequence freeShapes;
    stubDestDoc.shapeTool->GetFreeShapes(freeShapes);
    ASSERT_EQ(freeShapes.Length(), 2);
}

TEST_F(RegistryDocumentImporterIntegrationTest, AssemblyInDestDocHasCorrectPartsAfterImport) {
    stubSourceDoc.addAssembly();
    EXPECT_CALL(*std::static_pointer_cast<SpyShapeRegistry>(registry),
                registerShape(::testing::_, ::testing::_))
        .Times(3)
        .WillRepeatedly([&](const ShapeImportData& data, TDF_Label label) {
            return std::static_pointer_cast<SpyShapeRegistry>(
                registry
            )->realRegisterShape(data, label);
        });
    importer->importDocument(stubSourceDoc.document);

    TDF_LabelSequence freeShapes;
    stubDestDoc.shapeTool->GetFreeShapes(freeShapes);
    ASSERT_EQ(freeShapes.Length(), 1);
    auto assemblyLabel = freeShapes.Value(1);
    ASSERT_TRUE(stubDestDoc.shapeTool->IsAssembly(assemblyLabel));
    TDF_LabelSequence components;
    stubDestDoc.shapeTool->GetComponents(assemblyLabel, components);
    ASSERT_EQ(components.Length(), 2);
    for (Standard_Integer i = 1; i <= components.Length(); ++i) {
        EXPECT_FALSE(components.Value(i).IsNull());
    }
}