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
* File      : MGTMesh_MeshParameters.hpp
* Author    : Paweł Gilewicz
* Date      : 18/03/2025
*/

#ifndef MGTMESH_MESHPARAMETRS_HPP
#define MGTMESH_MESHPARAMETRS_HPP

#include <string>

class MGTMesh_MeshParameters {
public:
	explicit MGTMesh_MeshParameters() = default;
	~MGTMesh_MeshParameters() = default;

	enum Fineness { VeryCoarse, Coarse, Moderate, Fine, VeryFine, UserDefined };

public:
	Fineness fineness { Moderate };
	bool secondOrder {};
	bool quadAllowed {};

	// Mesh size
	double maxSize {}, minSize {};
	double growthRate {};
	std::string meshSizeFile;
	double nbSegPerRadius {};
	double nbSegPerEdge {};

	// Optimizer
	bool optimize {};
	int nbSurfOptSteps {};
	int nbVolOptSteps {};
	double elemSizeWeight {};
	int worstElemMeasure {};

	// Insider
	bool surfaceCurvature {};
	bool useDelauney {};
	bool checkOverlapping {};
	bool checkChartBoundary {};
};

#endif
