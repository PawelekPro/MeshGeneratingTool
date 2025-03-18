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
* File      : NetgenPlugin_Parameters.hpp
* Author    : Paweł Gilewicz
* Date      : 26/01/2025
*/

#ifndef NETGENPLUGIN_PARAMETERS_HPP
#define NETGENPLUGIN_PARAMETERS_HPP

#include "MGTMesh_Algorithm.hpp"
#include "NetgenPlugin_Defs.hpp"

#include <string>

class NETGENPLUGIN_EXPORT NetgenPlugin_Parameters final
	: public MGTMesh_Algorithm {
public:
	explicit NetgenPlugin_Parameters(int schemeId);
	explicit NetgenPlugin_Parameters(const MGTMesh_Algorithm& algorithm);

	static Fineness GetDefaultFineness() { return Moderate; }
	static bool GetDefaultSecondOrder() { return false; }
	static bool GetDefaultQuadAllowed() { return false; }
	static double GetDefaultMaxSize() { return 1000; }
	static double GetDefaultGrowthRate() { return 0.3; }
	static double GetDefaultNbSegPerRadius() { return 2; }
	static double GetDefaultNbSegPerEdge() { return 1; }
	static bool GetDefaultOptimize() { return true; }
	static int GetDefaultNbSurfOptSteps() { return 3; }
	static int GetDefaultNbVolOptSteps() { return 3; }
	static double GetDefaultElemSizeWeight() { return 0.2; }
	static int GetDefaultWorstElemMeasure() { return 2; }
	static bool GetDefaultSurfaceCurvature() { return true; }
	static bool GetDefaultUseDelauney() { return true; }
	static bool GetDefaultCheckOverlapping() { return true; }
	static bool GetDefaultCheckChartBoundary() { return true; }
};

#endif