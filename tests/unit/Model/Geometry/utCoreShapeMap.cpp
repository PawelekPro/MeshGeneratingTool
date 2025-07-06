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

class CoreShapeMapTest : public ::testing::Test {
protected:
    std::unique_ptr<ShapeCore> shapeCore;
    std::shared_ptr<const ShapeMap> map;
    void SetUp() override {
        shapeCore = std::make_unique<OcafShapeCore>();
        map = shapeCore->shapeMap();
        cube = StubShapes::cube();
        sphere = StubShapes::sphere();
        cubeSubShapes = StubShapes::subShapes(cube);
    }
    TopoDS_Shape cube;
    TopoDS_Shape sphere;
    TopTools_IndexedMapOfShape cubeSubShapes;
};

TEST_F(CoreShapeMapTest, TestMapReturnsAllFreeShapes){
    auto cubeKey = shapeCore->registerNewFreeShape(cube);
    auto sphereKey = shapeCore->registerNewFreeShape(sphere);
    auto cubeId = ShapeIdFactory::create(cubeKey);
    auto sphereId = ShapeIdFactory::create(sphereKey);

    auto idShapePairs = map->freeShapes();
   
    
    std::unordered_map<ShapeId, TopoDS_Shape, ShapeIdHasher> sourceMap;
    ShapeMap::ShapeIdPair cubePair(cubeId, cube);
    ShapeMap::ShapeIdPair spherePair(sphereId, sphere);
    
    sourceMap.insert(cubePair);
    sourceMap.insert(spherePair);

    ASSERT_EQ(idShapePairs.size(), 2);
    for( auto idShapePair : idShapePairs){
        auto id = idShapePair.first;
        auto shape = idShapePair.second;
        ASSERT_TRUE(sourceMap.contains(id));
        auto shapeFromMap = sourceMap.at(id);
        ASSERT_TRUE(shapeFromMap.IsSame(shape));
    }
};

TEST_F(CoreShapeMapTest, TestMapReturnsAllSubShapesOfAShape){
    auto cubeKey = shapeCore->registerNewFreeShape(cube);
    auto cubeId = ShapeIdFactory::create(cubeKey);
    auto mappedId = map->atShape(cube);
    auto mappedShape = map->atId(mappedId);
    auto mappedShapeCubeId = map->atId(cubeId);

    auto subShapes = StubShapes::subShapes(cube);
    int subshapecount = subShapes.Extent();

    auto idShapePairs = map->subShapes(cubeId);
    
    for( auto idShapePair : idShapePairs){
        auto id = idShapePair.first;
        auto shape = idShapePair.second;
        ASSERT_FALSE(shape.Free());
    }
};