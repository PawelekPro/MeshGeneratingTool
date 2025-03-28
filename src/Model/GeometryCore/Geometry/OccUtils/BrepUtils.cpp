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

#include "BrepUtils.hpp"

namespace BrepUtils
{

TopoDS_Face makeTriangleFace(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& p3){
    TopoDS_Face face; 
    if (!(!p1.IsEqual(p2, 0.0) && !p1.IsEqual(p3, 0.0))) {
        std::cerr << "Doubled points, returning empty face.";
    }

    TopoDS_Vertex v1 = BRepBuilderAPI_MakeVertex(p1);
    TopoDS_Vertex v2 = BRepBuilderAPI_MakeVertex(p2);
    TopoDS_Vertex v3 = BRepBuilderAPI_MakeVertex(p3);

    TopoDS_Wire wire = BRepBuilderAPI_MakePolygon(v1, v2, v3, Standard_True);
    face = BRepBuilderAPI_MakeFace(wire);
    
    return face;
};




}