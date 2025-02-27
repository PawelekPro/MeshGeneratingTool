/*
 * Copyright (C) 2024 Paweł Gilewicz
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

*=============================================================================
* File      : MGTMeshUtils_ControlPoint.cpp
* Author    : Paweł Gilewicz
* Date      : 24/11/2024
*/

#include "MGTMeshUtils_ControlPoint.h"

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Triangulation.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Ax3.hxx>
#include <gp_Dir.hxx>
#include <gp_Trsf.hxx>

namespace MGTMeshUtils {
// Surface sampling functions
void subdivideTriangle(const gp_Pnt& p1,
	const gp_Pnt& p2,
	const gp_Pnt& p3,
	const double& theSize,
	std::vector<ControlPoint>& thePoints);

void computePointsForSplitting(const gp_Pnt& p1,
	const gp_Pnt& p2,
	const gp_Pnt& p3,
	gp_Pnt midPoints[3]);

gp_Pnt tangencyPoint(const gp_Pnt& p1,
	const gp_Pnt& p2,
	const gp_Pnt& Center);
}

//----------------------------------------------------------------------------
void MGTMeshUtils::createPointsSampleFromFace(
	const TopoDS_Face& theFace, const double& theSize, std::vector<ControlPoint>& thePoints) {
	BRepMesh_IncrementalMesh M(theFace, 0.01, Standard_True);
	TopLoc_Location location;

	// Triangulate the face
	Handle(Poly_Triangulation) aTri = BRep_Tool::Triangulation(theFace, location);

	// Get the transformation associated to the face location
	gp_Trsf aTrsf = location.Transformation();

	// Get triangles
	int nbTriangles = aTri->NbTriangles();
	thePoints.reserve(thePoints.size() + nbTriangles);
	for (int i = 1; i <= nbTriangles; i++) {
		const Poly_Triangle& aTriangle = aTri->Triangle(i); // Get triangle by index
		gp_Pnt p1 = aTri->Node(aTriangle.Value(1)); // Get node by index
		gp_Pnt p2 = aTri->Node(aTriangle.Value(2)); // Get node by index
		gp_Pnt p3 = aTri->Node(aTriangle.Value(3)); // Get node by index

		p1.Transform(aTrsf);
		p2.Transform(aTrsf);
		p3.Transform(aTrsf);

		subdivideTriangle(p1, p2, p3, theSize, thePoints);
	}
}

//----------------------------------------------------------------------------
void MGTMeshUtils::subdivideTriangle(const gp_Pnt& p1,
	const gp_Pnt& p2,
	const gp_Pnt& p3,
	const double& theSize,
	std::vector<ControlPoint>& thePoints) {
	// Size threshold to stop subdividing
	// This value ensures that two control points are distant no more than 2*theSize
	double threshold = sqrt(3.) * theSize;

	if (p1.Distance(p2) > threshold || p2.Distance(p3) > threshold || p3.Distance(p1) > threshold)
		try {
			gp_Pnt midPoints[3];
			computePointsForSplitting(p1, p2, p3, midPoints);

			subdivideTriangle(midPoints[0], midPoints[1], midPoints[2], theSize, thePoints);
			subdivideTriangle(midPoints[0], p2, midPoints[1], theSize, thePoints);
			subdivideTriangle(midPoints[2], midPoints[1], p3, theSize, thePoints);
			subdivideTriangle(p1, midPoints[0], midPoints[2], theSize, thePoints);
			return;
		} catch (...) {
		}

	gp_Pnt massCenter = (p1.XYZ() + p2.XYZ() + p3.XYZ()) / 3.;
	thePoints.emplace_back(massCenter, theSize);
}

//----------------------------------------------------------------------------
void MGTMeshUtils::computePointsForSplitting(const gp_Pnt& p1,
	const gp_Pnt& p2,
	const gp_Pnt& p3,
	gp_Pnt midPoints[3]) {
	// Change coordinates
	gp_Trsf Trsf_1; // Identity transformation
	gp_Ax3 reference_system(gp::Origin(), gp::DZ(), gp::DX()); // OXY

	gp_Vec Vx(p1, p3);
	gp_Vec Vaux(p1, p2);
	gp_Dir Dx(Vx);
	gp_Dir Daux(Vaux);
	gp_Dir Dz = Dx.Crossed(Daux);
	gp_Ax3 current_system(p1, Dz, Dx);

	Trsf_1.SetTransformation(reference_system, current_system);

	gp_Pnt A = p1.Transformed(Trsf_1);
	gp_Pnt B = p2.Transformed(Trsf_1);
	gp_Pnt C = p3.Transformed(Trsf_1);

	double a = B.Distance(C);
	double b = A.Distance(C);
	double c = B.Distance(A);

	// Incenter coordinates
	// see http://mathworld.wolfram.com/Incenter.html
	double Xi = (b * B.X() + c * C.X()) / (a + b + c);
	double Yi = (b * B.Y()) / (a + b + c);
	gp_Pnt Center(Xi, Yi, 0);

	// Calculate the tangency points of the incircle
	gp_Pnt T1 = tangencyPoint(A, B, Center);
	gp_Pnt T2 = tangencyPoint(B, C, Center);
	gp_Pnt T3 = tangencyPoint(C, A, Center);

	midPoints[0] = T1.Transformed(Trsf_1.Inverted());
	midPoints[1] = T2.Transformed(Trsf_1.Inverted());
	midPoints[2] = T3.Transformed(Trsf_1.Inverted());

	return;
}

//----------------------------------------------------------------------------
gp_Pnt MGTMeshUtils::tangencyPoint(const gp_Pnt& p1,
	const gp_Pnt& p2,
	const gp_Pnt& Center) {
	double Xt = 0;
	double Yt = 0;

	// The tangency point is the intersection of the straight line (p1 p2)
	// and the straight line (Center T) which is orthogonal to (p1 p2)
	if (fabs(p1.X() - p2.X()) <= Precision::Confusion()) {
		Xt = p1.X(); // T is on (p1 p2)
		Yt = Center.Y(); // (Center T) is orthogonal to (p1 p2)
	} else if (fabs(p1.Y() - p2.Y()) <= Precision::Confusion()) {
		Yt = p1.Y(); // T is on (p1 p2)
		Xt = Center.X(); // (Center T) is orthogonal to (p1 p2)
	} else {
		// First straight line coefficients (equation y=a*x+b)
		double a = (p2.Y() - p1.Y()) / (p2.X() - p1.X());
		double b = p1.Y() - a * p1.X(); // p1 is on this straight line

		// Second straight line coefficients (equation y=c*x+d)
		double c = -1 / a; // The 2 lines are orthogonal
		double d = Center.Y() - c * Center.X(); // Center is on this straight line

		Xt = (d - b) / (a - c);
		Yt = a * Xt + b;
	}

	return gp_Pnt(Xt, Yt, 0);
}