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

class OcafShapeCoreTest : public ::testing::Test {
protected:
    std::unique_ptr<ShapeCore> shapeCore;
    
    void SetUp() override {
        shapeCore = std::make_unique<OcafShapeCore>();
        cube = StubShapes::cube();
        sphere = StubShapes::sphere();
        cubeSubShapes = StubShapes::subShapes(cube);
    }

    TopoDS_Shape cube;
    TopoDS_Shape sphere;
    TopTools_IndexedMapOfShape cubeSubShapes;
};

TEST_F(OcafShapeCoreTest, TestRegisteredShapeIsInShapeMap){
    shapeCore->registerNewFreeShape(cube);
    ASSERT_TRUE(shapeCore->shapeMap()->containsShape(cube));
};

TEST_F(OcafShapeCoreTest, TestUndoRegisterNewShapeRemovesShapesFromMap){
    shapeCore->openCommand();
    shapeCore->registerNewFreeShape(cube);
    shapeCore->commitCommand();
    shapeCore->undo();
    ASSERT_FALSE(shapeCore->shapeMap()->containsShape(cube));
};

TEST_F(OcafShapeCoreTest, TestRegisteredShapeSubShapesAreInMap){
    shapeCore->registerNewFreeShape(cube);
    auto shapeMap = shapeCore->shapeMap();
    for( size_t i = 1; i < cubeSubShapes.Extent(); i++ ){
        TopoDS_Shape shape = cubeSubShapes(i);
        ShapeId id = shapeMap->atShape(shape);
        ASSERT_TRUE(id.isValid());
    }
};

TEST_F(OcafShapeCoreTest, TestRegisterNewShapePublishesShapeAddedEvent){
    ShapeId id = shapeCore->registerNewFreeShape(cube);
    std::shared_ptr<const ShapeMap> shapeMap = shapeCore->shapeMap();
    ASSERT_TRUE(shapeMap->containsId(id));
};

TEST_F(OcafShapeCoreTest, TestRegisterNewShapePublishesShapeAddedEvent){
    ShapeId id = shapeCore->registerNewFreeShape(cube);
    std::shared_ptr<const ShapeMap> shapeMap = shapeCore->shapeMap();
    ASSERT_TRUE(shapeMap->containsId(id));
};

// TEST_F(OcafShapeCoreTest, TestUndoneRegisteredShapeIsNotInMap){
//     shapeCore.openCommand();
//     shapeCore.registerNewShape(cube);
//     shapeCore.commitCommand();

//     std::shared_ptr<const ShapeMap> shapeMap = shapeCore.shapeMap();
//     ASSERT_FALSE(shapeMap->containsShape(cube));
// };