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
#include "DefaultsParser.hpp"


// Test Fixture
class JsonValueToPropertyTest : public ::testing::Test {
protected:
    QDomDocument doc;

    QDomElement runTest(const char* json) {
        rapidjson::Document jsonDoc;
        jsonDoc.Parse(json);
        return DefaultsParser::jsonValueToProperty(jsonDoc);
    }
};

TEST_F(JsonValueToPropertyTest, MissingRequiredAttributes) {
    const char* json = R"({
        "label": "Element size",
        "hidden": "no"
    })";

    QDomElement result = runTest(json);
    ASSERT_TRUE(result.isNull());
}

TEST_F(JsonValueToPropertyTest, UnsupportedValueType) {
    const char* json = R"({
        "name": "testProperty",
        "unsupportedType": {}
    })";

    QDomElement result = runTest(json);
    ASSERT_FALSE(result.isNull());
    EXPECT_EQ(result.attribute("name"), "testProperty");
    EXPECT_FALSE(result.hasAttribute("unsupportedType"));
}

TEST_F(JsonValueToPropertyTest, EmptyJsonObject) {
    const char* json = R"({})";

    QDomElement result = runTest(json);
    ASSERT_TRUE(result.isNull());
}

TEST_F(JsonValueToPropertyTest, JsonWithValueAsBool) {
    const char* json = R"({
        "name": "isVisible",
        "value": true
    })";

    QDomElement result = runTest(json);
    ASSERT_FALSE(result.isNull());
    EXPECT_EQ(result.attribute("name"), "isVisible");
    EXPECT_EQ(result.attribute("value"), "true");
}