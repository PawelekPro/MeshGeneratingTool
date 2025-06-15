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
#include "MessageProgressIndicator.hpp"

class MessageTracker {
    public:
    std::vector<ProgressMessage> publishedMessages;

    void onBaseMessagePublished(const ProgressMessage& message) {
        publishedMessages.push_back(message);
    }
};

class MessageProgressIndicatorTest: public ::testing::Test {

    protected:
    std::unique_ptr<MessageBus> messageBus;
    std::unique_ptr<MessageTracker> messageTracker;
    std::unique_ptr<ProgressIndicator> ProgressIndicator;

    void SetUp() override {
        messageBus = std::make_unique<MessageBus>();
        messageTracker = std::make_unique<MessageTracker>();    
        messageBus->subscribe<ProgressMessage>(
            [this](const ProgressMessage& message) {
                messageTracker->onBaseMessagePublished(message);
            }
        );
        ProgressIndicator = std::make_unique<MessageProgressIndicator>(
            *messageBus
        );
    }
};

TEST_F(MessageProgressIndicatorTest, MessageProgressIndicatorPublishesProgress) {
    ProgressIndicator->progress("Begin Progress", 0);
    EXPECT_EQ(messageTracker->publishedMessages.size(), 1);
    EXPECT_EQ(messageTracker->publishedMessages[0].progressPercent, 0);
    EXPECT_EQ(messageTracker->publishedMessages[0].message, "Begin Progress");

    ProgressIndicator->progress("Progress", 50);
    EXPECT_EQ(messageTracker->publishedMessages.size(), 2);
    EXPECT_EQ(messageTracker->publishedMessages[1].progressPercent, 50);
    EXPECT_EQ(messageTracker->publishedMessages[1].message, "Progress");

    ProgressIndicator->progress("Finish Progress", 100);
    EXPECT_EQ(messageTracker->publishedMessages.size(), 3);
    EXPECT_EQ(messageTracker->publishedMessages[2].progressPercent, 100);
    EXPECT_EQ(messageTracker->publishedMessages[2].message, "Finish Progress");
}
