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
 *
 * Mesh      : C++ implementation
 * File      : MeshHDS_Document.h
 * Author    : Paweł Gilewicz
 * Date      : 26/10/2024
 */

#include "MeshHDS_Document.h"

#include <exception>

//----------------------------------------------------------------------------
MeshHDS_Document::MeshHDS_Document() { }

//----------------------------------------------------------------------------
MeshHDS_Document::~MeshHDS_Document() { }

//----------------------------------------------------------------------------
MeshHDS_Mesh* MeshHDS_Document::newMesh(int MeshID) {
	std::map<int, MeshHDS_Mesh*>::iterator i_m
		= _meshes.insert(make_pair(MeshID, (MeshHDS_Mesh*)0)).first;

	if (i_m->second)
		throw std::exception("MeshHDS_Document::newMesh(): ID of existing mesh given");
	MeshHDS_Mesh* mesh = new MeshHDS_Mesh(MeshID);
	i_m->second = mesh;
	return mesh;
}