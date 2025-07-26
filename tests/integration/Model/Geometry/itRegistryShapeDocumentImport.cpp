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
    return  arg.name == expected.name &&
            arg.color == expected.color &&
            arg.location == expected.location &&
            arg.shape == expected.shape;
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

TEST_F(RegistryDocumentImporterIntegrationTest, TestTrue) {
    // stubSourceDoc.addAssembly();
   
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