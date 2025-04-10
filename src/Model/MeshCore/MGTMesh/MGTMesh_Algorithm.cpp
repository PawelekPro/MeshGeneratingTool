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
* File      : MGTMesh_Algorithm.cpp
* Author    : Paweł Gilewicz
* Date      : 24/11/2024
*/

#include "MGTMesh_Algorithm.hpp"

#include "MGTMeshUtils_ComputeError.hpp"

#include <BRep_Tool.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Geom_Curve.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS_Edge.hxx>

//----------------------------------------------------------------------------
MGTMesh_Algorithm::MGTMesh_Algorithm(const int schemeId)
	: MGTMesh_Scheme(schemeId) {
	_error = COMPERR_OK;
}

//----------------------------------------------------------------------------
MGTMesh_Algorithm::~MGTMesh_Algorithm() = default;

//----------------------------------------------------------------------------
double MGTMesh_Algorithm::EdgeLength(const TopoDS_Edge& E) {
	double UMin = 0, UMax = 0;
	TopLoc_Location L;
	Handle(Geom_Curve) curve = BRep_Tool::Curve(E, L, UMin, UMax);
	if (curve.IsNull())
		return 0.;

	GeomAdaptor_Curve AdaptCurve(
		curve, UMin, UMax); // range is important for periodic curves
	double length = GCPnts_AbscissaPoint::Length(AdaptCurve, UMin, UMax);
	return length;
}