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
* File      : NetgenPlugin_NetgenLibWrapper.h
* Author    : Paweł Gilewicz
* Date      : 21/11/2024
*/
#ifndef NETGENPLUGIN_NETGENLIBWRAPPER_H
#define NETGENPLUGIN_NETGENLIBWRAPPER_H

#include "NetgenPlugin_Defs.hpp"

#include <fstream>
#include <ostream>

namespace nglib {
#include <nglib.h>
}

namespace netgen {
class OCCGeometry;
class Mesh;
}

struct NETGENPLUGIN_EXPORT NetgenPlugin_NetgenLibWrapper {
	NetgenPlugin_NetgenLibWrapper();
	~NetgenPlugin_NetgenLibWrapper();
	void setMesh(nglib::Ng_Mesh* mesh);
	nglib::Ng_Mesh* ngMesh() { return (nglib::Ng_Mesh*)(void*)_ngMesh; }

	static int GenerateMesh(netgen::OCCGeometry& occgeo, int startWith, int endWith, netgen::Mesh*& ngMesh);
	int GenerateMesh(netgen::OCCGeometry& occgeo, int startWith, int endWith) {
		return GenerateMesh(occgeo, startWith, endWith, _ngMesh);
	}

	static int& instanceCounter();

	bool _isComputeOk;
	netgen::Mesh* _ngMesh;

private:
	std::string getOutputFileName();
	void removeOutputFile();

private:
	std::ostream* _ngcout;
	std::ostream* _ngcerr;
	std::streambuf* _coutBuffer;
	std::string _outputFileName;
};

#endif