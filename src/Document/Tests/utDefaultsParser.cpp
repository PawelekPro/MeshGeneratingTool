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
#include "DocUtils.hpp"

class JsonValueToPropertyTest : public ::testing::Test {
protected:
    QDomDocument doc;

    QDomElement runTest(const char* json) {
        rapidjson::Document jsonDoc;
        jsonDoc.Parse(json);
        return DefaultsParser::jsonValueToProperty(doc, jsonDoc);
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
    EXPECT_EQ(Properties::getPropertyValue(result), "true");
}


class GetPropertiesFromJsonTest : public ::testing::Test {
protected:
    QDomDocument doc;

    QDomElement runTest(const char* json, const QString& jsonItemEntry) {
        rapidjson::Document jsonDoc;
        jsonDoc.Parse(json);
        return DefaultsParser::getPropertiesFromJson(doc, jsonDoc, jsonItemEntry);
    }
};

TEST_F(GetPropertiesFromJsonTest, MissingJsonEntry) {
    const char* json = R"({
        "SomeOtherEntry": {
            "Properties": []
        }
    })";

    QDomElement result = runTest(json, "MissingEntry");
    EXPECT_FALSE(result.hasChildNodes());
    EXPECT_EQ(result.tagName(), "Properties");
}

TEST_F(GetPropertiesFromJsonTest, MissingPropertiesKey) {
    const char* json = R"({
        "ValidEntry": {
            "OtherKey": "SomeValue"
        }
    })";

    QDomElement result = runTest(json, "ValidEntry");
    EXPECT_EQ(result.tagName(), "Properties");
    EXPECT_FALSE(result.hasChildNodes());
}

TEST_F(GetPropertiesFromJsonTest, PropertiesKeyNotArray) {
    const char* json = R"({
        "ValidEntry": {
            "Properties": "NotAnArray"
        }
    })";

    QDomElement result = runTest(json, "ValidEntry");
    EXPECT_EQ(result.tagName(), "Properties");
    EXPECT_FALSE(result.hasChildNodes());
}

TEST_F(GetPropertiesFromJsonTest, ValidPropertiesArray) {
    const char* json = R"({
        "ValidEntry": {
            "Properties": [
                { "name": "property1", "value": "value1" },
                { "name": "property2", "value": 42 },
                { "name": "property3", "value": true }
            ]
        }
    })";

    QDomElement result = runTest(json, "ValidEntry");
    EXPECT_EQ(result.tagName(), "Properties");
    EXPECT_EQ(result.childNodes().count(), 3);

    QDomElement property1 = result.firstChildElement("Property");
    EXPECT_EQ(property1.attribute("name"), "property1");
    EXPECT_EQ(Properties::getPropertyValue(property1), "value1");

    QDomElement property2 = property1.nextSiblingElement("Property");
    EXPECT_EQ(property2.attribute("name"), "property2");
    EXPECT_EQ(Properties::getPropertyValue(property2), "42");

    QDomElement property3 = property2.nextSiblingElement("Property");
    EXPECT_EQ(property3.attribute("name"), "property3");
    EXPECT_EQ(Properties::getPropertyValue(property3), "true");
}

TEST_F(GetPropertiesFromJsonTest, InvalidPropertyInArray) {
    const char* json = R"({
        "ValidEntry": {
            "Properties": [
                { "name": "property1", "value": "value1" },
                { "invalidKey": "invalidValue" }
            ]
        }
    })";

    QDomElement result = runTest(json, "ValidEntry");
    EXPECT_EQ(result.tagName(), "Properties");
    EXPECT_EQ(result.childNodes().count(), 1);

    QDomElement property1 = result.firstChildElement("Property");
    EXPECT_EQ(property1.attribute("name"), "property1");
    EXPECT_EQ(Properties::getPropertyValue(property1), "value1");
}

TEST_F(GetPropertiesFromJsonTest, EmptyPropertiesArray) {
    const char* json = R"({
        "ValidEntry": {
            "Properties": []
        }
    })";

    QDomElement result = runTest(json, "ValidEntry");
    EXPECT_EQ(result.tagName(), "Properties");
    EXPECT_FALSE(result.hasChildNodes());
}

TEST_F(GetPropertiesFromJsonTest, InvalidJsonStructure) {
    const char* json = R"({
        "ValidEntry": {
            "Properties": [
                { "name": "property1", "value": "value1" },
                "NotAnObject"
            ]
        }
    })";

    QDomElement result = runTest(json, "ValidEntry");
    EXPECT_EQ(result.tagName(), "Properties");
    EXPECT_EQ(result.childNodes().count(), 1);

    QDomElement property1 = result.firstChildElement("Property");
    EXPECT_EQ(property1.attribute("name"), "property1");
    EXPECT_EQ(Properties::getPropertyValue(property1), "value1");
}


class GetComboBoxListFromJsonTest : public ::testing::Test {
protected:
    QStringList runTest(const char* json, const QString& jsonItemEntry) {
        rapidjson::Document jsonDoc;
        jsonDoc.Parse(json);
        return DefaultsParser::getComboBoxListFromJson(jsonDoc, jsonItemEntry);
    }
};

TEST_F(GetComboBoxListFromJsonTest, ValidComboBoxJsonEntry){
    const char* json = R"({
        "EntitySelection": {
            "0": {
                "label": "Vertex"
            },
            "1": {
                "label": "Edge"
            },
            "2": {
                "label": "Face"
            },
            "3": {
                "label": "Shape"
            }
        }
    })";
    QStringList resultList = runTest(json, "EntitySelection");
    EXPECT_EQ(resultList.at(0), "Vertex");
    EXPECT_EQ(resultList.at(1), "Edge");
    EXPECT_EQ(resultList.at(2), "Face");
    EXPECT_EQ(resultList.at(3), "Shape");
}

TEST_F(GetComboBoxListFromJsonTest, DontSortComboBoxEntries){
 const char* json = R"({
        "EntitySelection": {
            "1": {
                "label": "Vertex"
            },
            "0": {
                "label": "Edge"
            },
            "3": {
                "label": "Face"
            },
            "2": {
                "label": "Shape"
            }
        }
    })";
    QStringList resultList = runTest(json, "EntitySelection");
    EXPECT_EQ(resultList.at(0), "Edge");
    EXPECT_EQ(resultList.at(1), "Vertex");
    EXPECT_EQ(resultList.at(2), "Shape");
    EXPECT_EQ(resultList.at(3), "Face");
}

TEST_F(GetComboBoxListFromJsonTest, InvalidComboBoxJsonEntry){
    const char* json = R"({
        "WrongLabel": {
            "0": {
                "label": "Vertex"
            }
        }
    })";

    QStringList resultList = runTest(json, "EntitySelection");
    EXPECT_TRUE(resultList.isEmpty());

    json = R"({
        "EntitySelection": {
            "0": {
                "wrongLabel": "Vertex"
            }
        }
    })";
    resultList = runTest(json, "EntitySelection");
    EXPECT_TRUE(resultList.isEmpty());
}