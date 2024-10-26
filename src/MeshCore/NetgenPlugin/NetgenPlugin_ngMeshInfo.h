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

#ifndef NetgenPlugin_ngMeshInfo_H
#define NetgenPlugin_ngMeshInfo_H

#include "NetgenPlugin_Macro.h"

namespace netgen {
class OCCGeometry;
class Mesh;
}

struct NetgenPlugin_ngMeshInfo {
	int _nbNodes, _nbSegments, _nbFaces, _nbVolumes;
	bool _elementsRemoved; // case where netgen can remove free nodes
	char* _copyOfLocalH;

	NetgenPlugin_ngMeshInfo(netgen::Mesh* ngMesh = 0, bool checkRemovedElems = false);
	void transferLocalH(netgen::Mesh* fromMesh, netgen::Mesh* toMesh);
	void restoreLocalH(netgen::Mesh* ngMesh);
};

#endif