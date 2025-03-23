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

#include "OccProgressIndicator.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ProgressIndicator.hpp" 
#include "OccProgressIndicator.hpp" 

using ::testing::_;
using ::testing::Eq;
using ::testing::StrEq;

class MockProgressIndicator : public ProgressIndicator {
public:
    MOCK_METHOD(void, begin, (const std::string& aMessage, int aMaxProgress), (const, override));
    MOCK_METHOD(void, progress, (const std::string& aMessage, int aProgress), (const, override));
    MOCK_METHOD(void, finish, (const std::string& aMessage), (const, override));
};

class OccProgressIndicatorTest : public ::testing::Test {
protected:
    ::testing::NiceMock<MockProgressIndicator> mockProgress;
    OccProgressIndicator progressIndicator{mockProgress};
};
TEST_F(OccProgressIndicatorTest, ProgressMessageGeneratesFormatsSubTasks) {
    Message_ProgressRange rootRange = progressIndicator.Start();
    Message_ProgressScope rootScope(rootRange, "RootTask", 100, false);
    Message_ProgressRange subRange = rootScope.Next(10);
    Message_ProgressScope subScope(subRange, "SubTask", 50, false);
    Message_ProgressRange subSubRange = subScope.Next(20);
    Message_ProgressScope subSubScope(subSubRange, "SubSubTask", 10, false);     
    subSubScope.Next(5);
    
    std::stringstream expectedMessage;
    expectedMessage << "RootTask: 2 / 100 -> SubTask: 10 / 50 -> SubSubTask: 5 / 10";
    EXPECT_CALL(mockProgress, progress(StrEq(expectedMessage.str()), Eq(2)));
    progressIndicator.Show(subSubScope, false);
    EXPECT_EQ(testing::Mock::VerifyAndClearExpectations(&mockProgress), true);
};
