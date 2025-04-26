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
#include "StdShapeMap.hpp"

class StdShapeMapTest : public ::testing::Test {
protected:
    StdShapeMap shapeMap;
    TopoDS_Shape cube = StubShapes::cube();
    TopoDS_Shape sphere = StubShapes::sphere();
    TopTools_IndexedMapOfShape subShapes = StubShapes::subShapes(cube);
};

TEST_F(StdShapeMapTest, TestRegisteredTopLevelShapesCanBeFetched){
    ShapeId cubeId = shapeMap.registerTopLevelShape(cube);
    ShapeId sphereId = shapeMap.registerTopLevelShape(sphere);
    
    TopoDS_Shape fetchedCube = shapeMap.atId(cubeId);
    ShapeId fetchedCubeId = shapeMap.getId(cube);
    
    TopoDS_Shape fetchedSphere = shapeMap.atId(sphereId);
    ShapeId fetchedSphereId = shapeMap.getId(sphere);
    
    ASSERT_EQ(cube, fetchedCube);
    ASSERT_EQ(cubeId, fetchedCubeId);

    ASSERT_EQ(sphere, fetchedSphere);
    ASSERT_EQ(sphereId, fetchedSphereId);
}

TEST_F(StdShapeMapTest, TestRegisteringRegisteredTopShapeReturnsInvalidId){
    ShapeId id = shapeMap.registerTopLevelShape(cube);
    ShapeId anotherId = shapeMap.registerTopLevelShape(cube);
    ASSERT_FALSE(anotherId.isValid());
}

TEST_F(StdShapeMapTest, TestRegisteringRegisteredSubShapeReturnsInvalidId){
    ShapeId parentId = shapeMap.registerTopLevelShape(cube);
    int subShapeIntId = 2;
    TopoDS_Shape subShape = subShapes(subShapeIntId);
    ShapeId subId = shapeMap.registerSubShape(
        subShape, parentId
    );
    ShapeId anotherSubId = shapeMap.registerSubShape(
        subShape, parentId
    );
    ASSERT_FALSE(anotherSubId.isValid());
}

TEST_F(StdShapeMapTest, TestRegisteredSubShapeCanBeFetched){
    int subShapeIntId = 2;
    TopoDS_Shape subShape = subShapes(subShapeIntId);
    
    ShapeId parentId = shapeMap.registerTopLevelShape(cube);
    ShapeId subId = shapeMap.registerSubShape(subShape, parentId);

    TopoDS_Shape fetchedSubShape = shapeMap.atId(subId);
    ASSERT_EQ(subShape, fetchedSubShape);
    ShapeId fetchedSubId = shapeMap.getId(subShape);
    ASSERT_EQ(subId, fetchedSubId);
}

TEST_F(StdShapeMapTest, TestContainsMethodsReturnsTrueForInsertedShape){
    ShapeId id = shapeMap.registerTopLevelShape(cube);
    ASSERT_TRUE(shapeMap.containsId(id));
    ASSERT_TRUE(shapeMap.containsShape(cube));
}

TEST_F(StdShapeMapTest, TestRemoveShapeRemovesTopIdAndShape){
    ShapeId id = shapeMap.registerTopLevelShape(cube);
    bool removed = shapeMap.removeShape(id);
    ASSERT_TRUE(removed);
    ASSERT_FALSE(shapeMap.containsId(id));
    ASSERT_FALSE(shapeMap.containsShape(cube));
}

TEST_F(StdShapeMapTest, TestRemovingNonExistingTopIdReturnsFalse){
    ShapeId id = shapeMap.registerTopLevelShape(cube);
    bool removed = shapeMap.removeShape(id);
    bool anotherRemoved = shapeMap.removeShape(id);
    ASSERT_TRUE(removed);
    ASSERT_FALSE(anotherRemoved);
}

TEST_F(StdShapeMapTest, TestRemoveShapeRemovesSubIdAndShape){
    int subShapeIntId = 2;
    TopoDS_Shape subShape = subShapes(subShapeIntId);
    ShapeId parentId = shapeMap.registerTopLevelShape(cube);
    ShapeId subId = shapeMap.registerSubShape(subShape, parentId);

    bool removed = shapeMap.removeShape(subId);
    ASSERT_TRUE(removed);
    ASSERT_FALSE(shapeMap.containsId(subId));
    ASSERT_FALSE(shapeMap.containsShape(subShape));
}

TEST_F(StdShapeMapTest, TestRemovingNonExistingSubIdReturnsFalse){
    int subShapeIntId = 2;
    TopoDS_Shape subShape = subShapes(subShapeIntId);
    ShapeId parentId = shapeMap.registerTopLevelShape(cube);
    ShapeId subId = shapeMap.registerSubShape(subShape, parentId);

    bool removed = shapeMap.removeShape(subId);
    bool anotherRemoved = shapeMap.removeShape(subId);
    ASSERT_TRUE(removed);
    ASSERT_FALSE(anotherRemoved);
}

TEST_F(StdShapeMapTest, TestUpdateShapeChangesStoredTopShape){
    ShapeId id = shapeMap.registerTopLevelShape(cube);
    shapeMap.updateShape(id, sphere);
    TopoDS_Shape fetchedShape = shapeMap.atId(id);
    ASSERT_EQ(fetchedShape, sphere);
}

TEST_F(StdShapeMapTest, TestUpdateShapeChangesStoredSubShape){
    int subShapeIntId = 2;
    TopoDS_Shape subShape = subShapes(subShapeIntId);
    ShapeId parentId = shapeMap.registerTopLevelShape(cube);
    ShapeId subId = shapeMap.registerSubShape(subShape, parentId);
    
    shapeMap.updateShape(subId, sphere);
    // TODO: I do not think this should be possible. We should validate ShapeId,
    //       if the parentId methods points to correct shape.
    TopoDS_Shape fetchedShape = shapeMap.atId(subId);
    ASSERT_EQ(fetchedShape, sphere);
}