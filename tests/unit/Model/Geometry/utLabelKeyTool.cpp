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

#include "LabelKeyTool.hpp"
#include "StubShapes.hpp"

#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Label.hxx>

class LabelKeyToolTest : public ::testing::Test {
    protected:
    Handle(TDocStd_Document) document;
    Handle(XCAFDoc_ShapeTool) shapeTool;

    void SetUp() {
        auto app = XCAFApp_Application::GetApplication();
        app->NewDocument("XmlXCAF", document);
        app->InitDocument(document);
        shapeTool = XCAFDoc_DocumentTool::ShapeTool(document->Main());
    }
};

TEST_F(LabelKeyToolTest, KeyToLabelToLabelToKeyAreEqual) {
    auto label = shapeTool->AddShape(StubShapes::cube()); 
    auto key = LabelKeyTool::keyFromLabel(label);
    auto fetchedLabel = LabelKeyTool::labelFromKey(document->Main(), key);
    EXPECT_EQ(label, fetchedLabel);
}