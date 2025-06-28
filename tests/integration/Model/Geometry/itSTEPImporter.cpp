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

class STEPImporterTest : public ::testing::Test {
protected:
    STEPImporter importer;
};

TEST_F(STEPImporterTest, TestImportingPartFlangeFile){
    std::string filePath = std::string(TESTS_DATA_PATH) + "/flange.stp";
    IdleProgressIndicator indicator;
    std::vector<std::pair<TopoDS_Shape, ShapeAttr>> shapes = 
        importer.importFreeShapes(filePath, indicator);
    EXPECT_EQ(shapes.size(), 1);
    EXPECT_EQ(shapes[0].second.color.r, 1);
    EXPECT_EQ(shapes[0].second.color.g, 1);
    EXPECT_EQ(shapes[0].second.color.b, 0);
}

TEST_F(STEPImporterTest, TestImportingPartFlangeFileIntoDoc){
    std::string filePath = std::string(TESTS_DATA_PATH) + "/flange.stp";
    IdleProgressIndicator indicator;
    auto doc = importer.importFreeShapesIntoDoc(filePath, indicator);
    TDF_LabelSequence freeShapes;
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(
        doc->Main()
    );
    shapeTool->GetFreeShapes(freeShapes);
    ASSERT_EQ(freeShapes.Size(), 1);
}

TEST_F(STEPImporterTest, TestImportingPartCubeFile){
    std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";
    IdleProgressIndicator indicator;
    std::vector<std::pair<TopoDS_Shape, ShapeAttr>> shapes = 
        importer.importFreeShapes(filePath, indicator);
    EXPECT_EQ(shapes.size(), 1);
}