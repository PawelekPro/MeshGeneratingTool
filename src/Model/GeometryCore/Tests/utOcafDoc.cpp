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
#include "IEventSubject.hpp"
#include "OcafDoc.hpp"

class MockEventSubject : public IEventSubject {
public:
    MOCK_METHOD(void, publishEvent, (const Event& aEvent), (const, override));
    MOCK_METHOD(void, attachObserver, (std::shared_ptr<EventObserver>), (override));
    MOCK_METHOD(void, detachObserver, (std::shared_ptr<EventObserver>), (override));
};

TEST(GeometryOcafDoc, ImportPublishesEvent) {
    // Arrange
    MockEventSubject mockSubject;
    std::string filePath = TESTS_RESOURCES_PATH + std::string("/tests/cube.stp");
    OcafDoc doc(mockSubject);

    // Act & Assert
    EXPECT_CALL(mockSubject, publishEvent(::testing::_)).Times(::testing::AtLeast(1));
    doc.importSTEP(filePath);
}

TEST(GeometryOcafDoc, ImportSTEPAddsShape) {
    // Arrange
    ::testing::NiceMock<MockEventSubject> mockSubject;
    ON_CALL(mockSubject, publishEvent(::testing::_)).WillByDefault(::testing::Return());
    std::string filePath = TESTS_RESOURCES_PATH + std::string("/tests/cube.stp");
    OcafDoc doc(mockSubject);

    // Act
    doc.importSTEP(filePath);

    // Assert
    std::vector<TopoDS_Shape> shapes = doc.getAllShapes();
    EXPECT_EQ(shapes.size(), 1);
}

TEST(GeometryOcafDoc, UndoImportSTEPRemovesShape) {
    // Arrange
    ::testing::NiceMock<MockEventSubject> mockSubject;
    ON_CALL(mockSubject, publishEvent(::testing::_)).WillByDefault(::testing::Return());
    std::string filePath = TESTS_RESOURCES_PATH + std::string("/tests/cube.stp");
    OcafDoc doc(mockSubject);
    doc.importSTEP(filePath);

    // Act
    doc.undo();

    // Assert
    std::vector<TopoDS_Shape> shapes = doc.getAllShapes();
    EXPECT_EQ(shapes.size(), 0);
}

TEST(GeometryOcafDoc, ImportSTLAddsShape) {
    // Arrange
    ::testing::NiceMock<MockEventSubject> mockSubject;
    ON_CALL(mockSubject, publishEvent(::testing::_)).WillByDefault(::testing::Return());
    std::string filePath = TESTS_RESOURCES_PATH + std::string("/tests/cube.stl");
    OcafDoc doc(mockSubject);

    // Act
    doc.importSTL(filePath);

    // Assert
    std::vector<TopoDS_Shape> shapes = doc.getAllShapes();
    EXPECT_EQ(shapes.size(), 1);
}

TEST(GeometryOcafDoc, UndoImportSTLRemovesShape) {
    // Arrange
    ::testing::NiceMock<MockEventSubject> mockSubject;
    ON_CALL(mockSubject, publishEvent(::testing::_)).WillByDefault(::testing::Return());
    std::string filePath = TESTS_RESOURCES_PATH + std::string("/tests/cube.stl");
    OcafDoc doc(mockSubject);
    doc.importSTL(filePath);

    // Act
    doc.undo();

    // Assert
    std::vector<TopoDS_Shape> shapes = doc.getAllShapes();
    EXPECT_EQ(shapes.size(), 0);
}
