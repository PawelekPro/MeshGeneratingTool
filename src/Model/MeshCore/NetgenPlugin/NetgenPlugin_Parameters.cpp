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
* File      : NetgenPlugin_Parameters.cpp
* Author    : Paweł Gilewicz
* Date      : 23/11/2024
*/

#include "NetgenPlugin_Parameters.h"

//----------------------------------------------------------------------------
NetgenPlugin_Parameters::NetgenPlugin_Parameters(int hypId)

	: _fineness(GetDefaultFineness())
	, _secondOrder(GetDefaultSecondOrder())
	, _quadAllowed(GetDefaultQuadAllowed())
	, _maxSize(GetDefaultMaxSize())
	, _minSize(0)
	, _growthRate(GetDefaultGrowthRate())
	, _nbSegPerRadius(GetDefaultNbSegPerRadius())
	, _nbSegPerEdge(GetDefaultNbSegPerEdge())
	, _optimize(GetDefaultOptimize())
	, _nbSurfOptSteps(GetDefaultNbSurfOptSteps())
	, _nbVolOptSteps(GetDefaultNbVolOptSteps())
	, _elemSizeWeight(GetDefaultElemSizeWeight())
	, _worstElemMeasure(GetDefaultWorstElemMeasure())
	, _surfaceCurvature(GetDefaultSurfaceCurvature())
	, _useDelauney(GetDefaultUseDelauney())
	, _checkOverlapping(GetDefaultCheckOverlapping())
	, _checkChartBoundary(GetDefaultCheckChartBoundary()) { }
