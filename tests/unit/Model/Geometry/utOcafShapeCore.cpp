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

#include "StubShapes.hpp"
#include "OcafShapeCore.hpp"

TEST(test,test){
    EXPECT_TRUE(true);
}

// class OcafShapeCoreTest : public ::testing::Test {
// protected:
//     OcafShapeCore shapeCore;
//     TopoDS_Shape cube = StubShapes::cube();
//     TopoDS_Shape sphere = StubShapes::sphere();
//     TopTools_IndexedMapOfShape subShapes = StubShapes::subShapes(cube);
// };

// TEST_F(OcafShapeCoreTest, TestRegisteredShapeIsShapeMap){
    // shapeCore.openCommand();
    // shapeCore.registerNewShape(cube);
    // TopTools_IndexedMapOfShape map;
    // TopExp::MapShapes(cube, map);
    // shapeCore.commitCommand();
    // shapeCore.undo();
    // std::shared_ptr<const ShapeMap> shapeMap = shapeCore.shapeMap();
    // ASSERT_TRUE(shapeMap->containsShape(cube));
// };

// TEST_F(OcafShapeCoreTest, TestRegisterNewShapePublishesShapeAddedEvent){
//     shapeCore.openCommand();
//     shapeCore.registerNewShape(cube);
//     shapeCore.commitCommand();

//     std::shared_ptr<const ShapeMap> shapeMap = shapeCore.shapeMap();
//     ASSERT_TRUE(shapeMap->containsShape(cube));
// };

// TEST_F(OcafShapeCoreTest, TestUndoneRegisteredShapeIsNotInMap){
//     shapeCore.openCommand();
//     shapeCore.registerNewShape(cube);
//     shapeCore.commitCommand();

//     std::shared_ptr<const ShapeMap> shapeMap = shapeCore.shapeMap();
//     ASSERT_FALSE(shapeMap->containsShape(cube));
// };