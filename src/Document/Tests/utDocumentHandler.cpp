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
#include "DocumentHandler.hpp"
#include "DocUtils.hpp"

TEST(DocumentHandlerTest, CreateRootItems) {
    DocumentHandler& doc = DocumentHandler::getInstance();

    QDomElement geometryElement = doc.createRootElement(ItemTypes::Root::Geometry);
    QDomElement meshElement = doc.createRootElement(ItemTypes::Root::Mesh);
    QDomElement solutionElement = doc.createRootElement(ItemTypes::Root::Solution);
    QDomElement resultsElement = doc.createRootElement(ItemTypes::Root::Results);

    ASSERT_FALSE(geometryElement.isNull());
    ASSERT_FALSE(meshElement.isNull());
    ASSERT_FALSE(solutionElement.isNull());
    ASSERT_FALSE(resultsElement.isNull());

    EXPECT_EQ(geometryElement.tagName(), "Geometry");
    EXPECT_EQ(meshElement.tagName(), "Mesh");
    EXPECT_EQ(solutionElement.tagName(), "Solution");
    EXPECT_EQ(resultsElement.tagName(), "Results");

    EXPECT_FALSE(geometryElement.firstChildElement("Properties").isNull());
    EXPECT_FALSE(meshElement.firstChildElement("Properties").isNull());
    EXPECT_FALSE(solutionElement.firstChildElement("Properties").isNull());
    EXPECT_FALSE(resultsElement.firstChildElement("Properties").isNull());
}



TEST(DocumentHandlerTest, CreateGeometrySubItems){

    DocumentHandler& doc = DocumentHandler::getInstance();
    QDomElement geometryElement = doc.createRootElement(ItemTypes::Root::Geometry);
    QDomElement importElement = doc.createSubElement(ItemTypes::Geometry::ImportSTEP, geometryElement);
    EXPECT_FALSE(importElement.isNull());
    EXPECT_FALSE(importElement.firstChildElement("Properties").isNull());
}

TEST(DocumentHandlerTest, CreateMeshSubItems){

    DocumentHandler& doc = DocumentHandler::getInstance();
    QDomElement meshElement = doc.createRootElement(ItemTypes::Root::Mesh);
    QDomElement sizingElement = doc.createSubElement(ItemTypes::Mesh::ElementSizing, meshElement);
    EXPECT_FALSE(sizingElement.isNull());
    EXPECT_FALSE(sizingElement.firstChildElement("Properties").isNull());
}