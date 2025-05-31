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
#include "MessageBus.hpp"
#include "MockMessages.hpp"

class MessageBusTest : public ::testing::Test {
protected:
    MessageBus bus;
    std::unique_ptr<MessageTracker> tracker;
    
    void SetUp() override {
        tracker = std::make_unique<MessageTracker>();
    }
};

TEST_F(MessageBusTest, messageBusCallsSubscribedMethods) {
    bus.subscribe<MockMessage>([this](const MockMessage& msg) {
        tracker->onMessagePublished(msg);
    });
    bus.subscribe<MockEvent>([this](const MockEvent& msg) {
        tracker->onEventPublished(msg);
    });
    bus.subscribe<MockRequest>([this](const MockRequest& req) {
        tracker->onRequestPublished(req);
    });

    MockMessage message{1};
    MockEvent event{2};
    MockRequest request{3};
    
    bus.publish(message);
    bus.publish(event);
    bus.publish(request);

    EXPECT_EQ(tracker->nMessagesPublished(), 1);
    EXPECT_EQ(tracker->calledMessageIds[0], 1);

    EXPECT_EQ(tracker->nEventsPublished(), 1);
    EXPECT_EQ(tracker->calledEventIds[0], 2);

    EXPECT_EQ(tracker->nRequestsPublished(), 1);
    EXPECT_EQ(tracker->calledRequestIds[0], 3);
}

TEST_F(MessageBusTest, messageBusCallsAllMethodsForSingleType) {
    bool called1 = false;
    bool called2 = false;
    bool called3 = false;

    bus.subscribe<MockMessage>([&](const MockMessage& msg) {
        called1 = true;
        EXPECT_EQ(msg._id, 1);
    });

    bus.subscribe<MockMessage>([&](const MockMessage& msg) {
        called2 = true;
        EXPECT_EQ(msg._id, 1);
    });

    bus.subscribe<MockMessage>([&](const MockMessage& msg) {
        called3 = true;
        EXPECT_EQ(msg._id, 1);
    });

    MockMessage message{1};
    bus.publish(message);

    EXPECT_TRUE(called1);
    EXPECT_TRUE(called2);
    EXPECT_TRUE(called3);
}