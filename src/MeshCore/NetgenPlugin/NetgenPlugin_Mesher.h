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
 */

#ifndef NetgenPlugin_MESHER_H
#define NetgenPlugin_MESHER_H

#include "NetgenPlugin_Macro.h"
#include "NetgenPlugin_ngMeshInfo.h"
#include "TopoDS_Shape.hxx"

namespace nglib {
#include <nglib.h>
}

namespace netgen {
class OCCGeometry;
class Mesh;
}

class MeshMGT_Mesh;

class NETGENPLUGIN_EXPORT NetgenPlugin_Mesher {
public:
	NetgenPlugin_Mesher(MeshMGT_Mesh* mesh, const TopoDS_Shape& shape, const bool isVolume);
	~NetgenPlugin_Mesher();

	void setDefaultParameters();

private:
	MeshMGT_Mesh* _mesh;
	const TopoDS_Shape& _shape;
	bool _isVolume;
	netgen::Mesh* _ngMesh;
	netgen::OCCGeometry* _ngOccGeom;
};

#endif