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

#include "TreeItemTypes.hpp"

namespace ItemTypes{

    QString label(const Sub & aSubItem) noexcept {
        return std::visit([](const auto& itemTypeEnum) -> QString {

            using itemType = std::decay_t<decltype(itemTypeEnum)>;

            if constexpr (std::is_same_v<itemType, Geometry>) {
                switch (itemTypeEnum) {
                    case Geometry::ImportSTEP: return "ImportSTEP";
                    case Geometry::ExtrudeFace: return "ExtrudeFace";
                    default: return "Unknown Geometry";
                }
            } else if constexpr (std::is_same_v<itemType, Mesh>) {
                switch (itemTypeEnum) {
                    case Mesh::ElementSizing: return "ElementSizing";
                    default: return "Unknown Mesh";
                }
            } else if constexpr (std::is_same_v<itemType, Solution>) {
                switch (itemTypeEnum) {
                    case Solution::BoundaryConditions: return "BoundaryConditions";
                    case Solution::SourceTerms: return "SourceTerms";
                    case Solution::Metrics: return "Metrics";
                    default: return "Unknown Solution";
                }
            } else if constexpr (std::is_same_v<itemType, Results>) {
                switch (itemTypeEnum) {
                    case Results::ContourMap: return "ContourMap";
                    case Results::Residuals: return "Residuals";
                    default: return "Unknown Results";
                }
            } else {
                return "Unknown Item Type";
            }

        }, aSubItem);
    }

    QString label(const Root & aRootItem) noexcept {
        switch (aRootItem) {
            case Root::Geometry: return "Geometry";
            case Root::Mesh: return "Mesh";
            case Root::Solution: return "Solution";
            case Root::Results: return "Results";
            default: return "Unknown Root";
        }
    }

    Root rootType(const Sub& aSubType) noexcept {
        return std::visit([](const auto& subTypeEnum) -> Root {
            using subType = std::decay_t<decltype(subTypeEnum)>;
            if constexpr (std::is_same_v<subType, Geometry>) {
                return Root::Geometry;
            } else if constexpr (std::is_same_v<subType, Mesh>) {
                return Root::Mesh;
            } else if constexpr (std::is_same_v<subType, Solution>) {
                return Root::Solution;
            } else if constexpr (std::is_same_v<subType, Results>) {
                return Root::Results;
            } else {
                throw std::runtime_error("Unknown Sub Type");
            }
        }, aSubType);
    }
}