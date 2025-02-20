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
#include "DocUtils.hpp"
#include <gtest/gtest.h>
#include <QDomDocument>
#include <QVariant>
#include <map>
#include <QString>

class PropertiesTest : public ::testing::Test {
protected:
    QDomDocument doc;
    QDomElement root, properties;

    std::map<QString, std::pair<QString, QVariant>> propertyMap = {
        {"intValueProp", {"int value", QVariant(1)}},
        {"doubleValueProp", {"double value", QVariant(0.1)}},
        {"strValueProp", {"str value", QVariant(QString("string"))}},
        {"nonValueProp", {"non value", QVariant(QString(""))}}
    };

    void SetUp() override {
        root = doc.createElement("Root");
        doc.appendChild(root);

        properties = doc.createElement("Properties");
        root.appendChild(properties);

        for (const auto &entry : propertyMap) {
            const QString &name = entry.first;
            const QString &label = entry.second.first;
            const QVariant &value = entry.second.second;
            addProperty(properties, name, label, value);
        }
    }

    void addProperty(QDomElement &parent, const QString &name, const QString &label, const QVariant &value) {
        QDomElement property = doc.createElement("Property");
        property.setAttribute("name", name);
        property.setAttribute("label", label);

        QString valueString = value.toString();
        if (!valueString.isEmpty()) {
            QDomText textNode = doc.createTextNode(valueString);
            property.appendChild(textNode);
        }
        parent.appendChild(property);
    }
};

TEST_F(PropertiesTest, GetPropertyValue) {
    for (const auto &entry : propertyMap) {
        const QString &name = entry.first;
        const QVariant &expectedValue = entry.second.second;

        QDomElement property = Properties::getProperty(root, name);
        QString actualValue = Properties::getPropertyValue(property);
        EXPECT_EQ(expectedValue.toString(), actualValue);

        property = Properties::getProperty(properties, name);
        actualValue = Properties::getPropertyValue(property);
        EXPECT_EQ(expectedValue.toString(), actualValue);
    }
};

TEST_F(PropertiesTest, SetPropertyValue) {
    for (const auto& entry : propertyMap) {
        const QString& name = entry.first;
        const QVariant& originalValue = entry.second.second;
        const QString newValue = "Updated " + originalValue.toString();

        QDomElement property = Properties::getProperty(properties, name);
        Properties::setPropertyValue(property, newValue);
        QString updatedValue = Properties::getPropertyValue(property);
        EXPECT_EQ(newValue, updatedValue);
    }
};

TEST_F(PropertiesTest, GetPropertyAttribute) {
    for (const auto& entry : propertyMap) {
        const QString& name = entry.first;
        const QString& expectedLabel = entry.second.first;

        QDomElement property = Properties::getProperty(properties, name);
        ASSERT_FALSE(property.isNull());

        QString label = Properties::getPropertyAttribute(property, "label");
        EXPECT_EQ(expectedLabel, label);
    }
};

TEST_F(PropertiesTest, SetPropertyAttribute) {
    for (const auto& entry : propertyMap) {
        const QString& name = entry.first;

        QDomElement property = Properties::getProperty(properties, name);
        ASSERT_FALSE(property.isNull());

        QString newLabel = "New Label for " + name;
        Properties::setPropertyAttribute(property, "label", newLabel);

        QString updatedLabel = Properties::getPropertyAttribute(property, "label");
        EXPECT_EQ(newLabel, updatedLabel);
    }
};