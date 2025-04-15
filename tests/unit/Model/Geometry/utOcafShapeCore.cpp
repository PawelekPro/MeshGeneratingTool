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
    OcafShapeCore shapeCore;
    TopoDS_Shape cube = StubShapes::cube();
    TopoDS_Shape sphere = StubShapes::sphere();
    TopTools_IndexedMapOfShape subShapes = StubShapes::subShapes(cube);
};

TEST_F(OcafShapeCoreTest, TestRegisteringNewShapeIncrementsModifiedCount){
    shapeCore.openCommand();
    shapeCore.registerNewShape(cube);
    shapeCore.commitCommand();

    std::shared_ptr<const ShapeMap> shapeMap = shapeCore.shapeMap();
    ShapeId id = shapeMap->getId(cube);
    
    shapeCore.write("preUndo.xml");
    // shapeCore.openCommand();
    // shapeCore.removeShape(id);
    // shapeCore.commitCommand();
    shapeCore.undo();
    shapeCore.write("postUndo.xml");
}