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
#include "MessageProgressIndicator.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ShapeEventTracker {
    public:
    std::vector<ShapeAddedEvent> addEvents;
    std::vector<ShapeRemovedEvent> removeEvents;
    std::vector<ProgressMessage> progressMessages;

    void onRemovedEvent(const ShapeRemovedEvent& event) {
        removeEvents.push_back(event);
    }

    void onAddEvent(const ShapeAddedEvent& event) {
        addEvents.push_back(event);
    }

    void onProgressMessage(const ProgressMessage& event) {
        progressMessages.push_back(event);
    }
};

class ShapeServiceTest : public ::testing::Test {
    protected:
    std::unique_ptr<MessageBus> messageBus;
    std::unique_ptr<ShapeEventTracker> shapeEventTracker;

    std::shared_ptr<ShapeCore> shapeCore;
    std::unique_ptr<ShapeService> shapeService;

    void SetUp() override {
        messageBus = std::make_unique<MessageBus>();
        shapeEventTracker = std::make_unique<ShapeEventTracker>();
        
        messageBus->subscribe<ShapeAddedEvent>(
            [this](const ShapeAddedEvent& event) {
                shapeEventTracker->onAddEvent(event);
            }
        );
        messageBus->subscribe<ShapeRemovedEvent>(
            [this](const ShapeRemovedEvent& event) {
                shapeEventTracker->onRemovedEvent(event);
            }
        );

        messageBus->subscribe<ProgressMessage>(
            [this](const ProgressMessage& event) {
                shapeEventTracker->onProgressMessage(event);
            }
        );
        
        shapeCore = std::make_shared<OcafShapeCore>();
        shapeService = std::make_unique<ShapeService>(*messageBus, shapeCore);
    }

};

TEST_F(ShapeServiceTest, ShapeServicePublishesShapeAddedEventOnSingleSTEPImport) {
    // Arrange
    std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";
   
    // Act 
    shapeService->importSTEP(filePath);
    
    // Assert
    auto publishedId = shapeEventTracker->addEvents[0].shapeId;
    ASSERT_EQ(shapeEventTracker->addEvents.size(), 1);
    ASSERT_TRUE(shapeCore->shapeMap()->containsId(publishedId));
    ASSERT_FALSE(shapeCore->shapeMap()->atId(publishedId).IsNull());
}

TEST_F(ShapeServiceTest, ShapeServicePublishesShapeAddedEventOnMultiPartSTEPImport) {
    // Arrange
    std::string filePath = std::string(TESTS_DATA_PATH) + "/flange.stp";

    // Act 
    shapeService->importSTEP(filePath);

    // Assert
    ASSERT_EQ(shapeEventTracker->addEvents.size(), 3);
    for (auto event : shapeEventTracker->addEvents){
        ASSERT_TRUE(shapeCore->shapeMap()->containsId(event.shapeId));
        ASSERT_FALSE(shapeCore->shapeMap()->atId(event.shapeId).IsNull());
    }
}

TEST_F(ShapeServiceTest, ShapeServicePublishesProgressMessagesOnSETPImport) {
    // Arrange
    std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";
   
    // Act 
    shapeService->importSTEP(filePath);
    
    // Assert
    ASSERT_TRUE(shapeEventTracker->progressMessages.size() > 0);
}

TEST_F(ShapeServiceTest, ShapeServicePublishesOnSingleRemoveShape) {
    // Arrange
    std::string filePath = std::string(TESTS_DATA_PATH) + "/cube.stp";
    shapeService->importSTEP(filePath);
    auto importedShapeId = shapeEventTracker->addEvents[0].shapeId;
   
    // Act 
    shapeService->removeShape(importedShapeId);
    
    // Assert
    ASSERT_EQ(shapeEventTracker->removeEvents.size(), 1);
    ASSERT_FALSE(shapeCore->shapeMap()->containsId(importedShapeId));
}

TEST_F(ShapeServiceTest, ShapeServicePublishesOnMultiRemoveShape) {
    // Arrange
    std::string filePath = std::string(TESTS_DATA_PATH) + "/flange.stp";
    shapeService->importSTEP(filePath);
   
    // Act 
    for (auto event : shapeEventTracker->addEvents){
        shapeService->removeShape(event.shapeId);
    }
    
    // Assert
    ASSERT_EQ(shapeEventTracker->removeEvents.size(), 3);
    for (auto event : shapeEventTracker->removeEvents){
        ASSERT_FALSE(shapeCore->shapeMap()->containsId(event.shapeId));
    }
}