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
* File      : MGTMeshUtils_ViscousLayers.h
* Author    : Paweł Gilewicz
* Date      : 27/11/2024
*/
#ifndef MGTMESHUTILS_VISCOUSLAYERS_H
#define MGTMESHUTILS_VISCOUSLAYERS_H

// #include "MGTMesh_Scheme.h"

// #include <vector>

// class MGTMeshUtils_ViscousLayers : public MGTMesh_Scheme {
// public:
// 	enum ExtrusionMethod {
// 		// node is translated along normal to a surface with possible further smoothing
// 		SURF_OFFSET_SMOOTH,
// 		// node is translated along the average normal of surrounding faces till
// 		// intersection with a neighbor face translated along its own normal
// 		// by the layers thickness
// 		FACE_OFFSET,
// 		// node is translated along the average normal of surrounding faces
// 		// by the layers thickness
// 		NODE_OFFSET
// 	};

// public:
// 	MGTMeshUtils_ViscousLayers(int schemeId);

// 	static const char* GetSchemeType() { return "ViscousLayers"; }

// private:
// 	std::vector<int> _shapeIds;
// 	bool _isToIgnoreShapes;
// 	int _nbLayers;
// 	double _thickness;
// 	double _stretchFactor;
// 	ExtrusionMethod _method;
// 	std::string _groupName;
// };

#endif