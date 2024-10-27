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
 * NetgenPlugin : C++ implementation
 * File      	: NetgenPlugin_LibWrapper.h
 * Author    	: Paweł Gilewicz
 * Date      	: 06/10/2024
 */

#ifndef NetgenPlugin_LibWrapper_H
#define NetgenPlugin_LibWrapper_H

#include "NetgenPlugin_Macro.h"

namespace nglib {
#include <nglib.h>
}

#include <string>

namespace netgen {
class OCCGeometry;
class Mesh;
}

struct NETGENPLUGIN_EXPORT NetgenPlugin_LibWrapper {
	bool _isComputeOk;
	netgen::Mesh* _ngMesh;

	NetgenPlugin_LibWrapper();
	~NetgenPlugin_LibWrapper();

	void setMesh(nglib::Ng_Mesh* mesh);
	nglib::Ng_Mesh* ngMesh();
	static int generateMesh(netgen::OCCGeometry& occgeo, int startWith, int endWith, netgen::Mesh*& ngMesh);
	int generateMesh(netgen::OCCGeometry& occgeo, int startWith, int endWith);
	static void calcLocalH(netgen::Mesh* ngMesh);
	static void removeTmpFiles();
	static int& instanceCounter();

private:
	std::string getOutputFileName();
	void removeOutputFile();

	std::string _outputFileName;
	std::ostream* _ngcout;
	std::ostream* _ngcerr;
	std::streambuf* _coutBuffer; // to re-/store cout.rdbuf()
};

#endif