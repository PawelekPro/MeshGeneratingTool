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
 * File      : NetgenPlugin_LibWrapper.cpp
 * Author    : Paweł Gilewicz
 * Date      : 06/10/2024
 */

#include "NetgenPlugin_LibWrapper.h"

// Netgen include files
#ifndef OCCGEOMETRY
#define OCCGEOMETRY
#endif
#include <meshing.hpp>
#include <occgeom.hpp>

namespace {
inline void NOOP_Deleter(void*) { ; }
};

namespace netgen {
NETGENPLUGIN_DLL_HEADER
extern MeshingParameters mparam;
};

//----------------------------------------------------------------------------
NetgenPlugin_LibWrapper::NetgenPlugin_LibWrapper()
	: _ngMesh(0) {
	if (instanceCounter() == 0) {
		nglib::Ng_Init();
		if (!netgen::testout)
			netgen::testout = new ofstream("test.out");
	}

	++instanceCounter();

	_isComputeOk = false;
	_coutBuffer = nullptr;
	_ngcout = nullptr;
	_ngcerr = nullptr;
	if (!getenv("KEEP_NETGEN_OUTPUT")) {
		// redirect all netgen output (mycout,myerr,cout) to _outputFileName
		_outputFileName = getOutputFileName();
		_ngcout = netgen::mycout;
		_ngcerr = netgen::myerr;
		netgen::mycout = new ofstream(_outputFileName.c_str());
		netgen::myerr = netgen::mycout;
		_coutBuffer = std::cout.rdbuf();
#ifdef _DEBUG_
		std::cout << "NOTE: netgen output is redirected to file " << _outputFileName << std::endl;
#else
		std::cout.rdbuf(netgen::mycout->rdbuf());
#endif
	}

	setMesh(nglib::Ng_NewMesh());
}

//----------------------------------------------------------------------------
NetgenPlugin_LibWrapper::~NetgenPlugin_LibWrapper() {
	--instanceCounter();

	nglib::Ng_DeleteMesh(ngMesh());
	nglib::Ng_Exit();
	removeTmpFiles();
	if (_coutBuffer)
		std::cout.rdbuf(_coutBuffer);
	removeOutputFile();
}

//----------------------------------------------------------------------------
void NetgenPlugin_LibWrapper::setMesh(nglib::Ng_Mesh* mesh) {
	if (_ngMesh)
		nglib::Ng_DeleteMesh(ngMesh());
	_ngMesh = (netgen::Mesh*)mesh;
}

//----------------------------------------------------------------------------
int NetgenPlugin_LibWrapper::generateMesh(netgen::OCCGeometry& occgeo,
	int startWith, int endWith, netgen::Mesh*& ngMesh) {
	int err = 0;
	if (!ngMesh)
		ngMesh = new netgen::Mesh;

	ngMesh->SetGeometry(shared_ptr<netgen::NetgenGeometry>(&occgeo, &NOOP_Deleter));

	netgen::mparam.perfstepsstart = startWith;
	netgen::mparam.perfstepsend = endWith;
	std::shared_ptr<netgen::Mesh> meshPtr(ngMesh, &NOOP_Deleter);
	err = occgeo.GenerateMesh(meshPtr, netgen::mparam);

	return err;
}

//----------------------------------------------------------------------------
void NetgenPlugin_LibWrapper::calcLocalH(netgen::Mesh* ngMesh) {
#if defined(NETGEN_V5) || defined(NETGEN_V6)
	ngMesh->CalcLocalH(netgen::mparam.grading);
#endif
}

//----------------------------------------------------------------------------
void NetgenPlugin_LibWrapper::removeTmpFiles() {
	// FIXME: Add files handling utilities
	// 	bool rm = SMESH_File("test.out").remove();
	// #ifndef WIN32
	// 	if (rm && netgen::testout && instanceCounter() == 0) {
	// 		delete netgen::testout;
	// 		netgen::testout = 0;
	// 	}
	// #endif
	// 	SMESH_File("problemfaces").remove();
	// 	SMESH_File("occmesh.rep").remove();
}

//----------------------------------------------------------------------------
int& NetgenPlugin_LibWrapper::instanceCounter() {
	static int theCouner = 0;
	return theCouner;
}

//----------------------------------------------------------------------------
std::string NetgenPlugin_LibWrapper::getOutputFileName() {
	// FIXME: Definition of temporary directory in MeshGeneratingTool project
	std::string aTmpDir = "";

	TCollection_AsciiString aGenericName = aTmpDir.c_str();
	aGenericName += "NETGEN_";
#ifndef WIN32
	aGenericName += getpid();
#else
	aGenericName += _getpid();
#endif
	aGenericName += "_";
	aGenericName += Abs((Standard_Integer)(long)aGenericName.ToCString());
	aGenericName += ".out";

	return aGenericName.ToCString();
}

//----------------------------------------------------------------------------
void NetgenPlugin_LibWrapper::removeOutputFile() {
	if (!_outputFileName.empty()) {
		if (_ngcout) {
			delete netgen::mycout;
			netgen::mycout = _ngcout;
			netgen::myerr = _ngcerr;
			_ngcout = 0;
		}
		// FIXME: Directory handling utils
		// string tmpDir = SALOMEDS_Tool::GetDirFromPath(_outputFileName);
		// string aFileName = SALOMEDS_Tool::GetNameFromPath(_outputFileName) + ".out";
		// SALOMEDS_Tool::ListOfFiles aFiles;
		// aFiles.reserve(1);
		// aFiles.push_back(aFileName.c_str());

		// SALOMEDS_Tool::RemoveTemporaryFiles(tmpDir.c_str(), aFiles, true);
	}
}

//----------------------------------------------------------------------------
int NetgenPlugin_LibWrapper::generateMesh(
	netgen::OCCGeometry& occgeo, int startWith, int endWith) {
	return this->generateMesh(occgeo, startWith, endWith, _ngMesh);
}

//----------------------------------------------------------------------------
nglib::Ng_Mesh* NetgenPlugin_LibWrapper::ngMesh() {
	return (nglib::Ng_Mesh*)(void*)_ngMesh;
}