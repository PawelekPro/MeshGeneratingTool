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
* File      : NetgenPlugin_Mesher.hpp
* Author    : Paweł Gilewicz
* Date      : 21/11/2024
*/
#ifndef NETGENPLUGIN_MESHER_H
#define NETGENPLUGIN_MESHER_H

#include "NetgenPlugin_Defs.hpp"

namespace netgen {
class OCCGeometry;
class Mesh;
}

class gp_XYZ;
class TopoDS_Shape;
class MGTMesh_MeshObject;
class MGTMeshUtils_ViscousLayers;
class NetgenPlugin_Parameters;

class NETGENPLUGIN_EXPORT NetgenPlugin_Mesher {
public:
	NetgenPlugin_Mesher(
		MGTMesh_MeshObject* mesh,
		const TopoDS_Shape& shape,
		const NetgenPlugin_Parameters* algorithm);
	~NetgenPlugin_Mesher();
	bool ComputeMesh();

	static void PrepareOCCgeometry(
		netgen::OCCGeometry& occgeom, const TopoDS_Shape& shape);
	double GetDefaultMinSize(const TopoDS_Shape& geom, const double maxSize);

	static void RestrictLocalSize(
		netgen::Mesh& ngMesh, const gp_XYZ& p, double size, const bool overrideMinH = true);
	static void SetLocalSize(netgen::OCCGeometry& occgeo, netgen::Mesh& ngMesh);

	void SetDefaultParameters();
	void SetParameters(const MGTMeshUtils_ViscousLayers* layersScheme);

private:
	MGTMesh_MeshObject* _mesh;
	const TopoDS_Shape& _shape;
	const NetgenPlugin_Parameters* _algorithm;
	bool _optimize;
	int _fineness;
	bool _isViscousLayers2D;

	netgen::Mesh* _ngMesh;
	netgen::OCCGeometry* _occgeom;

	const MGTMeshUtils_ViscousLayers* _viscousLayers;

	// a pointer to NetgenPlugin_Mesher* field of the holder, that will be
	// nullified at destruction of this
	NetgenPlugin_Mesher** _selfPtr;
};

#endif