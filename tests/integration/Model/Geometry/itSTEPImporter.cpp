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

#include "STEPImporter.hpp"
#include "ShapeImporter.hpp"

#include <iostream>
#include <sstream>
#include <TDF_LabelSequence.hxx>
#include <Standard_Handle.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>

class STEPImporterCubeTest : public ::testing::Test {
protected:
    static inline std::string filePath;
    static inline Handle(TDocStd_Document) doc;
    STEPImporter importer;
    
    static void SetUpTestSuite() {
        filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";
    }

    static void TearDownTestSuite() {
        doc.Nullify();
    }
};

class STEPImporterJointTest : public ::testing::Test {
protected:
    static inline std::string filePath;
    static inline Handle(TDocStd_Document) doc;
    STEPImporter importer;

    static void SetUpTestSuite() {
        filePath = std::string(TESTS_DATA_PATH) + "/joint.stp";
    }

    static void TearDownTestSuite() {
        doc.Nullify();
    }
};

TEST_F(STEPImporterCubeTest, TestImportCubeFile) {
    doc = importer.import(filePath);
} 

TEST_F(STEPImporterCubeTest, TestImportedDocHasOneShapeOnly) {
    ASSERT_FALSE(doc.IsNull());

    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

    TDF_LabelSequence labels;
    shapeTool->GetFreeShapes(labels);

    EXPECT_EQ(labels.Length(), 1);
}

TEST_F(STEPImporterJointTest, TestImportJointTest) {
    doc = importer.import(filePath);
} 

TEST_F(STEPImporterJointTest, TestImportedDocHasOneAssemblyWith2SubParts) {
    ASSERT_FALSE(doc.IsNull());

    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

    TDF_LabelSequence roots;
    shapeTool->GetFreeShapes(roots);

    EXPECT_EQ(roots.Length(), 1);

    TDF_Label rootAssembly = roots.First();

    EXPECT_TRUE(shapeTool->IsAssembly(rootAssembly));

    TDF_LabelSequence children;
    shapeTool->GetComponents(rootAssembly, children);
    
    EXPECT_EQ(children.Length(), 2);
}