/*
 * Copyright (C) 2024 Krystian Fudali
 *
 * This file is part of the Mesh Generating Tool.
 * (https://github.com/PawelekPro/MeshGeneratingTool)
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
#ifndef DOCITEMTYPES_HPP
#define DOCITEMTYPES_HPP

#include <QString>
#include <variant>
#include <stdexcept>

namespace ItemTypes {

enum class Root { Geometry, Mesh, Solution, Results };

enum class Geometry {
	ImportSTEP,
	ExtrudeFace,
};

enum class Mesh { ElementSizing };

enum class Solution { BoundaryConditions, SourceTerms, Metrics };

enum class Results { ContourMap, Residuals };

using Sub = std::variant<Geometry, Mesh, Solution, Results>;

QString label(const Sub& aSubItem) noexcept;
QString label(const Root& aRootItem) noexcept;

Root rootType(const Sub& aSubType) noexcept;
}

#endif