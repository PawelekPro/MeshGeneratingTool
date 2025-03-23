/*
* Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
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

*=============================================================================
* File      : MGTMesh_DefaultParameters.cpp
* Author    : Paweł Gilewicz
* Date      : 23/03/2024
*/

#include "MGTMeshUtils_DefaultParameters.hpp"

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_B3d.hxx>
#include <Poly_Triangulation.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include <cmath>

//----------------------------------------------------------------------------
void updateTriangulation(const TopoDS_Shape& shape) {

	try {
		BRepMesh_IncrementalMesh e(shape, 0.01, true);
	} catch (Standard_Failure&) { }
}

//----------------------------------------------------------------------------
double MGTMeshUtils_DefaultParameters::GetDefaultMinSize(
	const TopoDS_Shape& geom, const double maxSize) {
	updateTriangulation(geom);

	TopLoc_Location loc;
	int i1, i2, i3;
	double minh = 1e100;
	Bnd_B3d bb;

	for (TopExp_Explorer fExp(geom, TopAbs_FACE); fExp.More(); fExp.Next()) {
		Handle(Poly_Triangulation) triangulation
			= BRep_Tool::Triangulation(TopoDS::Face(fExp.Current()), loc);
		if (triangulation.IsNull())
			continue;

		const double fTol = BRep_Tool::Tolerance(TopoDS::Face(fExp.Current()));
		const Standard_Integer numTriangles = triangulation->NbTriangles();

		for (Standard_Integer iT = 1; iT <= numTriangles; ++iT) {
			Poly_Triangle triangle = triangulation->Triangle(iT);
			triangle.Get(i1, i2, i3);

			gp_Pnt p1 = triangulation->Node(i1);
			gp_Pnt p2 = triangulation->Node(i2);
			gp_Pnt p3 = triangulation->Node(i3);

			// Calculate distances between vertices
			const double dist2_1 = p1.SquareDistance(p2);
			const double dist2_2 = p2.SquareDistance(p3);
			const double dist2_3 = p3.SquareDistance(p1);

			// Check and update minimum distance
			if (dist2_1 < minh && fTol * fTol < dist2_1)
				minh = dist2_1;
			if (dist2_2 < minh && fTol * fTol < dist2_2)
				minh = dist2_2;
			if (dist2_3 < minh && fTol * fTol < dist2_3)
				minh = dist2_3;

			// Update bounding box
			bb.Add(p1);
			bb.Add(p2);
			bb.Add(p3);
		}
	}

	if (minh > 0.25 * bb.SquareExtent()) {
		minh = 1e-3 * sqrt(bb.SquareExtent());
	} else {
		minh = sqrt(minh);
	}

	if (minh > 0.5 * maxSize)
		minh = maxSize / 3.0;

	return minh;
}