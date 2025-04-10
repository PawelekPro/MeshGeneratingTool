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
* File      : NetgenPlugin_Parameters.cpp
* Author    : Paweł Gilewicz
* Date      : 23/11/2024
*/

#include "NetgenPlugin_Parameters.hpp"

//----------------------------------------------------------------------------
NetgenPlugin_Parameters::NetgenPlugin_Parameters(const int schemeId)
	: MGTMesh_Algorithm(schemeId) {
	fineness = GetDefaultFineness();
	secondOrder = GetDefaultSecondOrder();
	quadAllowed = GetDefaultQuadAllowed();
	maxSize = GetDefaultMaxSize();
	minSize = 0;
	growthRate = GetDefaultGrowthRate();
	nbSegPerRadius = GetDefaultNbSegPerRadius();
	nbSegPerEdge = GetDefaultNbSegPerEdge();
	optimize = GetDefaultOptimize();
	nbSurfOptSteps = GetDefaultNbSurfOptSteps();
	nbVolOptSteps = GetDefaultNbVolOptSteps();
	elemSizeWeight = GetDefaultElemSizeWeight();
	worstElemMeasure = GetDefaultWorstElemMeasure();
	surfaceCurvature = GetDefaultSurfaceCurvature();
	useDelauney = GetDefaultUseDelauney();
	checkOverlapping = GetDefaultCheckOverlapping();
	checkChartBoundary = GetDefaultCheckChartBoundary();
}

//----------------------------------------------------------------------------
NetgenPlugin_Parameters::NetgenPlugin_Parameters(
	const MGTMesh_Algorithm& algorithm)
	: MGTMesh_Algorithm(algorithm) {

	// Explicitly copy attributes
	fineness = algorithm.fineness;
	secondOrder = algorithm.secondOrder;
	quadAllowed = algorithm.quadAllowed;
	maxSize = algorithm.maxSize;
	minSize = algorithm.minSize;
	growthRate = algorithm.growthRate;
	nbSegPerRadius = algorithm.nbSegPerRadius;
	nbSegPerEdge = algorithm.nbSegPerEdge;
	optimize = algorithm.optimize;
	nbSurfOptSteps = algorithm.nbSurfOptSteps;
	nbVolOptSteps = algorithm.nbVolOptSteps;
	elemSizeWeight = algorithm.elemSizeWeight;
	worstElemMeasure = algorithm.worstElemMeasure;
	surfaceCurvature = algorithm.surfaceCurvature;
	useDelauney = algorithm.useDelauney;
	checkOverlapping = algorithm.checkOverlapping;
	checkChartBoundary = algorithm.checkChartBoundary;
}