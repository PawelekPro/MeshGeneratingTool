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

#ifndef STUBSHAPES_HPP
#define STUBSHAPES_HPP 

#include <TopExp.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <memory>
#include <vector>

#include "ShapeKey.hpp"
#include "ShapeCoreObserver.hpp"

namespace StubShapes{
    inline TopTools_IndexedMapOfShape subShapes(const TopoDS_Shape& aShape){
        TopTools_IndexedMapOfShape subShapes;
        TopExp::MapShapes(aShape, subShapes);
        return subShapes;
    }

    inline TopoDS_Shape cube() {
        return BRepPrimAPI_MakeBox(1.0, 1.0, 1.0).Shape();
    }

    inline TopoDS_Shape sphere() {
        return BRepPrimAPI_MakeSphere(1.0).Shape();
    }
}


#endif