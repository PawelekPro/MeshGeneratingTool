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
#include "EventProgressIndicator.hpp"
#include "ModelEvents.hpp"
#include "ModelSubject.hpp"

class MockModelSubject : public ModelSubject {
public:
    MOCK_METHOD(void, publishEvent, (const Event& event), (const override));
};

class ProgressIndicatorTest : public ::testing::Test{
protected:
    MockModelSubject subject;
    EventProgressIndicator indicator{subject};
};

MATCHER_P3(MatchProgressEvent, 
    expectedLabel, 
    expectedValue, 
    expectedState, 
    "") {
    const auto* progressEvent = dynamic_cast<const ProgressEvent*>(&arg);
    return progressEvent &&
           progressEvent->label == expectedLabel &&
           progressEvent->value == expectedValue &&
           progressEvent->state == expectedState;
}

TEST_F(ProgressIndicatorTest, IndicatorPublishesBeginEventOnBegin) {
    ProgressEvent expectedEvent("begin", 100, ProgressState::Begin);
    
    EXPECT_CALL(subject, publishEvent(MatchProgressEvent(
        "begin", 100, ProgressState::Begin)));

    indicator.begin("begin", 100);
    
    EXPECT_EQ(testing::Mock::VerifyAndClearExpectations(&subject), true);
}

TEST_F(ProgressIndicatorTest, IndicatorPublishesProgressEventOnProgress) {
    ProgressEvent expectedEvent("progress", 50, ProgressState::Progress);

    EXPECT_CALL(subject, publishEvent(MatchProgressEvent(
        "progress", 50, ProgressState::Progress)));
    
    indicator.progress("progress", 50);
    
    EXPECT_EQ(testing::Mock::VerifyAndClearExpectations(&subject), true);
}

TEST_F(ProgressIndicatorTest, IndicatorPublishesFinishEventOnFinish) {
    ProgressEvent expectedEvent("finish", 100, ProgressState::Finish);
    
    EXPECT_CALL(subject, publishEvent(MatchProgressEvent(
        "finish", 100, ProgressState::Finish)));
    
    indicator.finish("finish");
    
    EXPECT_EQ(testing::Mock::VerifyAndClearExpectations(&subject), true);
}