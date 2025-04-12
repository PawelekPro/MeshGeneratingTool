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
#include "OcafShapeCore.hpp"
#include <iostream>
#include <sstream>

class OcafShapeCoreTest : public ::testing::Test {
protected:
    OcafShapeCore shapeCore;
};

TEST_F(OcafShapeCoreTest, TestRegisterNewShape){
    std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";
    IdleProgressIndicator indicator;
    STEPImporter importer;
    std::vector<std::pair<TopoDS_Shape, ShapeAttr>> shapes = 
        importer.importFile(filePath, indicator);
    TopoDS_Shape cube = shapes[0].first;

    const ShapeId shapeId = shapeCore.registerNewShape(cube);
    shapeCore.write(std::string("test.xml"));
}