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

#include "Geometry.hpp"
#include "OcafShapeCore.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

// class GeometryTest : public ::testing::Test {
//     protected:

//     CommandStack commandStack;
//     MessageBus messageBus;
//     std::unique_ptr<Geometry> geometry;
//     std::shared_ptr<ShapeCore> shapeCore;

//     void SetUp() override {
//         shapeCore = std::make_shared<OcafShapeCore>();
//         geometry = std::make_unique<Geometry>(
//             commandStack, messageBus, shapeCore
//         );
//     } 
// };

// TEST_F(GeometryTest, ImportSTEPFile) {
//     // Arrange
//     std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";

//     // Act
//     geometry->importSTEP(filePath);

//     // Assert
//     auto shapeMap = geometry->shapeView()->shapeMap();
//     ASSERT_EQ(shapeMap->freeShapes().size(), 1);    
// }

// TEST_F(GeometryTest, UndoImportSTEPFile) {
//     // Arrange
//     std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";

//     // Act
//     geometry->importSTEP(filePath);
//     commandStack.undo();

//     // Assert
//     auto shapeMap = geometry->shapeView()->shapeMap();
//     ASSERT_EQ(shapeMap->freeShapes().size(), 0);    
// }

// TEST_F(GeometryTest, RedoImportSTEPFile) {
//     // Arrange
//     std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";

//     // Act
//     geometry->importSTEP(filePath);
//     commandStack.undo();
//     commandStack.redo();

//     // Assert
//     auto shapeMap = geometry->shapeView()->shapeMap();
//     ASSERT_EQ(shapeMap->freeShapes().size(), 1);    
// }