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
#include "DocItemTypes.hpp"

using namespace ItemTypes;

TEST(ItemTypesTest, LabelSubItems) {
    EXPECT_EQ(label(Geometry::ImportSTEP), "ImportSTEP");
    EXPECT_EQ(label(Geometry::ExtrudeFace), "ExtrudeFace");
    EXPECT_EQ(label(Mesh::ElementSizing), "ElementSizing");
    EXPECT_EQ(label(Solution::BoundaryConditions), "BoundaryConditions");
    EXPECT_EQ(label(Solution::SourceTerms), "SourceTerms");
    EXPECT_EQ(label(Solution::Metrics), "Metrics");
    EXPECT_EQ(label(Results::ContourMap), "ContourMap");
    EXPECT_EQ(label(Results::Residuals), "Residuals");
}

TEST(ItemTypesTest, LabelRootItems) {
    EXPECT_EQ(label(Root::Geometry), "Geometry");
    EXPECT_EQ(label(Root::Mesh), "Mesh");
    EXPECT_EQ(label(Root::Solution), "Solution");
    EXPECT_EQ(label(Root::Results), "Results");
}

TEST(ItemTypesTest, RootTypeFunction) {
    EXPECT_EQ(rootType(Geometry::ImportSTEP), Root::Geometry);
    EXPECT_EQ(rootType(Geometry::ExtrudeFace), Root::Geometry);
    EXPECT_EQ(rootType(Mesh::ElementSizing), Root::Mesh);
    EXPECT_EQ(rootType(Solution::BoundaryConditions), Root::Solution);
    EXPECT_EQ(rootType(Solution::SourceTerms), Root::Solution);
    EXPECT_EQ(rootType(Solution::Metrics), Root::Solution);
    EXPECT_EQ(rootType(Results::ContourMap), Root::Results);
    EXPECT_EQ(rootType(Results::Residuals), Root::Results);
}