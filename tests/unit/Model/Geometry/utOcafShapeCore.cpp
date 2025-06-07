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

#include "GeometryStubs.hpp"
#include "OcafShapeCore.hpp"

class OcafShapeCoreTest : public ::testing::Test {
protected:
    std::unique_ptr<ShapeCore> shapeCore;
    std::shared_ptr<SpyShapeCoreObserver> observer = std::make_shared<SpyShapeCoreObserver>();
    void SetUp() override {
        shapeCore = std::make_unique<OcafShapeCore>();
        shapeCore->attachObserver(observer);
        cube = StubShapes::cube();
        sphere = StubShapes::sphere();
        cubeSubShapes = StubShapes::subShapes(cube);
    }
    TopoDS_Shape cube;
    TopoDS_Shape sphere;
    TopTools_IndexedMapOfShape cubeSubShapes;
};

TEST_F(OcafShapeCoreTest, TestRegisterNewShapeIsInMap){
    std::shared_ptr<ShapeKey> key = shapeCore->registerNewFreeShape(cube);
    std::shared_ptr<const ShapeMap> shapeMap = shapeCore->shapeMap();
    ShapeId id = ShapeIdFactory::create(key);
    ASSERT_TRUE(shapeMap->containsId(id));
    ASSERT_TRUE(shapeMap->containsShape(cube));
};

TEST_F(OcafShapeCoreTest, TestRegisteredShapeSubShapesAreInMap){
    shapeCore->registerNewFreeShape(cube);
    auto shapeMap = shapeCore->shapeMap();
    for( size_t i = 1; i < cubeSubShapes.Extent(); i++ ){
        TopoDS_Shape shape = cubeSubShapes(i);
        ShapeId id = shapeMap->atShape(shape);
        ASSERT_TRUE(shapeMap->containsId(id));
        ASSERT_TRUE(shapeMap->containsShape(shape));
    }
};

TEST_F(OcafShapeCoreTest, TestUndoRegisterNewShapeRemovesShapesFromMap){
    shapeCore->openCommand();
    shapeCore->registerNewFreeShape(cube);
    shapeCore->commitCommand();
    
    auto shapeMap = shapeCore->shapeMap();
    ShapeId id = shapeMap->atShape(cube);

    shapeCore->undo();

    ASSERT_FALSE(shapeMap->containsShape(cube));
    ASSERT_FALSE(shapeMap->containsId(id));
};

TEST_F(OcafShapeCoreTest, TestRedoRegisterNewShapeReturnsShapeToMap){
    shapeCore->openCommand();
    shapeCore->registerNewFreeShape(cube);
    shapeCore->commitCommand();
    
    shapeCore->undo();
    shapeCore->redo();

    auto shapeMap = shapeCore->shapeMap();
    
    ShapeId id = shapeMap->atShape(cube);
    ASSERT_TRUE(shapeMap->containsId(id));
    ASSERT_TRUE(shapeMap->containsShape(cube));
};

TEST_F(OcafShapeCoreTest, TestRegisterNewShapePublishesShapeAddedEvent){
    shapeCore->openCommand();
    auto key = shapeCore->registerNewFreeShape(cube);
    shapeCore->commitCommand();

    ASSERT_EQ(observer->shapeAddedPublished.size(), 1);
    auto publishedKey = observer->shapeAddedPublished[0];
    ASSERT_TRUE(*key == *publishedKey);
};

TEST_F(OcafShapeCoreTest, TestUndoRegisterNewShapePublishesShapeRemovedEvent){
    shapeCore->openCommand();
    auto key = shapeCore->registerNewFreeShape(cube);
    shapeCore->commitCommand();
    shapeCore->undo();
    ASSERT_EQ(observer->shapeRemovedPublished.size(), 1);
    auto publishedKey = observer->shapeRemovedPublished[0]; 
    ASSERT_TRUE(*key == *publishedKey); 
};

TEST_F(OcafShapeCoreTest, TestRedoRegisterNewShapePublishesShapeAddedEvent){
    shapeCore->openCommand();
    auto key = shapeCore->registerNewFreeShape(cube);
    shapeCore->commitCommand();
    shapeCore->undo();
    shapeCore->redo();
    ASSERT_EQ(observer->shapeAddedPublished.size(), 2);
    auto publishedKey = observer->shapeAddedPublished[1]; 
    ASSERT_TRUE(*key == *publishedKey); 
};