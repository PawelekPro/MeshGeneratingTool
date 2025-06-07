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
// #include "LabelShapeMap.hpp"

// #include <TDocStd_Document.hxx>
// #include <XCAFApp_Application.hxx>
// #include <XCAFDoc_ShapeTool.hxx>
// #include <XCAFDoc_DocumentTool.hxx>

// class TestableLabelShapeMap : public LabelShapeMap {
// public:
//     TestableLabelShapeMap()
//     : LabelShapeMap(initShapeTool())  // call base constructor with shapeTool
//     {}
//     using LabelShapeMap::registerTopLevelShape;
//     using LabelShapeMap::registerSubShape;
//     using LabelShapeMap::removeShape;
//     using LabelShapeMap::updateShape;

// private:
//     static Handle(XCAFDoc_ShapeTool) initShapeTool() {
//         Handle(XCAFApp_Application) app = XCAFApp_Application::GetApplication();
//         Handle(TDocStd_Document) document;
//         app->NewDocument("XmlXCAF", document);
//         app->InitDocument(document); // not strictly needed after NewDocument, but safe
//         document->SetUndoLimit(5);
//         return XCAFDoc_DocumentTool::ShapeTool(document->Main());
//     }
// };

// class LabelShapeMapTest : public ::testing::Test {
//     protected:
//     TestableLabelShapeMap shapeMap;
//     TopoDS_Shape cube = StubShapes::cube();
//     TopoDS_Shape sphere = StubShapes::sphere();
//     TopTools_IndexedMapOfShape subShapes = StubShapes::subShapes(cube);
// };

// TEST_F(LabelShapeMapTest, TestRegisteredTopLevelShapesCanBeFetched){
//     ShapeId cubeId = shapeMap.registerTopLevelShape(cube);
//     ShapeId sphereId = shapeMap.registerTopLevelShape(sphere);
    
//     TopoDS_Shape fetchedCube = shapeMap.atId(cubeId);
//     ShapeId fetchedCubeId = shapeMap.getId(cube);
    
//     TopoDS_Shape fetchedSphere = shapeMap.atId(sphereId);
//     ShapeId fetchedSphereId = shapeMap.getId(sphere);
    
//     ASSERT_EQ(cube, fetchedCube);
//     ASSERT_EQ(cubeId, fetchedCubeId);

//     ASSERT_EQ(sphere, fetchedSphere);
//     ASSERT_EQ(sphereId, fetchedSphereId);
// }

// TEST_F(LabelShapeMapTest, TestRegisteringRegisteredTopShapeReturnsExistingId){
//     ShapeId id = shapeMap.registerTopLevelShape(cube);
//     ShapeId anotherId = shapeMap.registerTopLevelShape(cube);
//     ASSERT_EQ(id, anotherId);
// }

// TEST_F(LabelShapeMapTest, TestRegisteringRegisteredSubShapeReturnsExistingId){
//     ShapeId parentId = shapeMap.registerTopLevelShape(cube);
//     int subShapeIntId = 2;
//     TopoDS_Shape subShape = subShapes(subShapeIntId);
//     ShapeId subId = shapeMap.registerSubShape(
//         subShape, parentId
//     );
//     ShapeId anotherSubId = shapeMap.registerSubShape(
//         subShape, parentId
//     );
//     ASSERT_EQ(subId, anotherSubId);
// }

// TEST_F(LabelShapeMapTest, TestRegisteredSubShapeCanBeFetched){
//     int subShapeIntId = 2;
//     TopoDS_Shape subShape = subShapes(subShapeIntId);
    
//     ShapeId parentId = shapeMap.registerTopLevelShape(cube);
//     ShapeId subId = shapeMap.registerSubShape(subShape, parentId);

//     TopoDS_Shape fetchedSubShape = shapeMap.atId(subId);
//     ASSERT_EQ(subShape, fetchedSubShape);
//     ShapeId fetchedSubId = shapeMap.getId(subShape);
//     ASSERT_EQ(subId, fetchedSubId);
// }

// TEST_F(LabelShapeMapTest, TestContainsMethodsReturnsTrueForInsertedShape){
//     ShapeId id = shapeMap.registerTopLevelShape(cube);
//     ASSERT_TRUE(shapeMap.containsId(id));
//     ASSERT_TRUE(shapeMap.containsShape(cube));
// }

// TEST_F(LabelShapeMapTest, TestRemoveShapeRemovesTopIdAndShape){
//     ShapeId id = shapeMap.registerTopLevelShape(cube);
//     bool removed = shapeMap.removeShape(id);
//     ASSERT_TRUE(removed);
//     ASSERT_FALSE(shapeMap.containsId(id));
//     ASSERT_FALSE(shapeMap.containsShape(cube));
// }

// TEST_F(LabelShapeMapTest, TestRemovingNonExistingTopIdReturnsFalse){
//     ShapeId id = shapeMap.registerTopLevelShape(cube);
//     bool removed = shapeMap.removeShape(id);
//     bool anotherRemoved = shapeMap.removeShape(id);
//     ASSERT_TRUE(removed);
//     ASSERT_FALSE(anotherRemoved);
// }

// TEST_F(LabelShapeMapTest, TestRemoveShapeReturnsFalseOnSubShape){
//     int subShapeIntId = 2;
//     TopoDS_Shape subShape = subShapes(subShapeIntId);
//     ShapeId parentId = shapeMap.registerTopLevelShape(cube);
//     ShapeId subId = shapeMap.registerSubShape(subShape, parentId);
    
//     bool removed = shapeMap.removeShape(subId);
//     ASSERT_FALSE(removed);
//     ASSERT_TRUE(shapeMap.containsId(subId));
//     ASSERT_TRUE(shapeMap.containsShape(subShape));
// }

// TEST_F(LabelShapeMapTest, TestUpdateShapeChangesStoredTopShape){
//     ShapeId id = shapeMap.registerTopLevelShape(cube);
//     shapeMap.updateShape(id, sphere);
//     TopoDS_Shape fetchedShape = shapeMap.atId(id);
//     ASSERT_EQ(fetchedShape, sphere);
// }

// TEST_F(LabelShapeMapTest, TestUpdateShapeChangesStoredSubShape){
//     int subShapeIntId = 2;
//     TopoDS_Shape subShape = subShapes(subShapeIntId);
//     ShapeId parentId = shapeMap.registerTopLevelShape(cube);
//     ShapeId subId = shapeMap.registerSubShape(subShape, parentId);
    
//     shapeMap.updateShape(subId, sphere);
//     // TODO: I do not think this should be possible. We should validate ShapeId,
//     //       if the parentId methods points to correct shape.
//     TopoDS_Shape fetchedShape = shapeMap.atId(subId);
//     ASSERT_EQ(fetchedShape, sphere);
// }