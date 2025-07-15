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

#ifndef SHAPEREGISTRY_HPP
#define SHAPEREGISTRY_HPP

#include <TDF_Label.hxx>
#include "Shape.hpp"
#include "ShapeImportData.hpp"

class ShapeRegistry {
    public:

    virtual ~ShapeRegistry() = default;

    virtual std::shared_ptr<Shape> registerShape(
        const ShapeImportData& aShapeData,
        TDF_Label aLocalParent = TDF_Label{}
    ) = 0;

    virtual TDF_Label baseLabel() = 0;

};
#endif