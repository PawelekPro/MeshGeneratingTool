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

#include "ShapeService.hpp"
#include "OcafShapeCore.hpp"
#include "GeometryEvents.hpp"
#include "ShapeEventTracker.hpp"
#include "MockCubeImporter.hpp"
#include "MockCubeAssemblyImporter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ShapeServiceTest : public ::testing::Test {
protected:
    static inline std::unique_ptr<MessageBus> messageBus;
    static inline std::unique_ptr<ShapeEventTracker> shapeEventTracker;
    static inline std::shared_ptr<ShapeCore> shapeCore;
    static inline std::unique_ptr<ShapeService> shapeService;

    static void SetUpTestSuite() {
        messageBus = std::make_unique<MessageBus>();
        shapeEventTracker = std::make_unique<ShapeEventTracker>(*messageBus);
        shapeCore = std::make_shared<OcafShapeCore>();
        shapeService = std::make_unique<ShapeService>(*messageBus, shapeCore);
    }

    static void TearDownTestSuite() {
        messageBus.reset();
        shapeEventTracker.reset();
        shapeCore.reset();
        shapeService.reset();
    }
};

class ShapeServiceCubeImportTest : public ShapeServiceTest {
protected:
    static void SetUpTestSuite() {
        ShapeServiceTest::SetUpTestSuite();
        MockCubeImporter importer;
        shapeService->importShapes(importer, "mockPath");
    }
    static inline ShapeId importedShapeId = ShapeId::invalidId();
};

class ShapeServiceJointImportTest : public ShapeServiceTest {
protected:
    static void SetUpTestSuite() {
        ShapeServiceTest::SetUpTestSuite();
        MockCubesAssemblyImporter importer;
        shapeService->importShapes(importer, "mockPath");
    }
    static inline ShapeId importedAssemblyId = ShapeId::invalidId();
    static inline std::vector<ShapeId> importedShapes;
};

TEST_F(ShapeServiceCubeImportTest, ShapeMapContainsImportedShape) {
    ASSERT_EQ(shapeEventTracker->shapeAddedEvents.size(), 1);
    ShapeServiceCubeImportTest::importedShapeId = \
        shapeEventTracker->shapeAddedEvents[0].shapeId;
    ASSERT_EQ(shapeCore->shapeMap()->freeShapes().size(), 1);
    ASSERT_TRUE(shapeCore->shapeMap()->containsId(importedShapeId));
    ASSERT_FALSE(shapeCore->shapeMap()->atId(importedShapeId).IsNull());
};

TEST_F(ShapeServiceCubeImportTest, ProgressMessagesArePublished) {
    ASSERT_TRUE(shapeEventTracker->progressMessages.size() > 0);
};

TEST_F(ShapeServiceCubeImportTest, ShapeAddedMessageIsPublishedWithCorrectId) {
    ASSERT_EQ(shapeEventTracker->shapeAddedEvents.size(), 1);
};

TEST_F(ShapeServiceJointImportTest, ShapeMapContainsImportedShapes) {
    ASSERT_EQ(shapeEventTracker->assemblyAddedEvents.size(), 1);
    ShapeServiceJointImportTest::importedAssemblyId = \
        shapeEventTracker->assemblyAddedEvents[0].shapeId;

    ASSERT_EQ(shapeEventTracker->shapeAddedEvents.size(), 2);
    ShapeServiceJointImportTest::importedShapes.push_back(
        shapeEventTracker->shapeAddedEvents[0].shapeId
    );
    ShapeServiceJointImportTest::importedShapes.push_back(
        shapeEventTracker->shapeAddedEvents[1].shapeId
    );
}

TEST_F(ShapeServiceJointImportTest, ShapeMapContainsImportedShape) {
    ASSERT_EQ(shapeCore->shapeMap()->freeShapes().size(), 1);

    ASSERT_TRUE(shapeCore->shapeMap()->containsId(importedAssemblyId));
    ASSERT_FALSE(shapeCore->shapeMap()->atId(importedAssemblyId).IsNull());


    ASSERT_TRUE(shapeCore->shapeMap()->containsId(importedShapes[0]));
    ASSERT_TRUE(shapeCore->shapeMap()->containsId(importedShapes[1]));
    
    ASSERT_FALSE(shapeCore->shapeMap()->atId(importedShapes[0]).IsNull());
    ASSERT_FALSE(shapeCore->shapeMap()->atId(importedShapes[1]).IsNull());
}

TEST_F(ShapeServiceJointImportTest, ProgressMessagesArePublished) {
    ASSERT_TRUE(shapeEventTracker->progressMessages.size() > 0);
}