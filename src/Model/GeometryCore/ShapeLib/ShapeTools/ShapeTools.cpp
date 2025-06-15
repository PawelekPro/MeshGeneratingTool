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

#include "ShapeTools.hpp"
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pnt.hxx>
#include <stdexcept>    

TopoDS_Shape ShapeTools::scaleShape(
    const TopoDS_Shape& aShape, 
    float aScaleFactor
) {
    if (aScaleFactor <= 0.0f) {
        throw std::invalid_argument("Scale factor must be greater than zero.");
    }

    gp_Trsf scaleTransformation;
    scaleTransformation.SetScale(gp_Pnt(0, 0, 0), aScaleFactor);

    BRepBuilderAPI_Transform transformer(aShape, scaleTransformation);
    
    return transformer.Shape();
}   