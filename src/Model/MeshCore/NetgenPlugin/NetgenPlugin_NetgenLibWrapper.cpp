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
* File      : NetgenPlugin_NetgenLibWapper.h
* Author    : Paweł Gilewicz
* Date      : 21/11/2024
*/

#include "NetgenPlugin_NetgenLibWrapper.h"

#include <TCollection_AsciiString.hxx>

#include <filesystem>

namespace nglib {
#include <nglib.h>
#include <nglib_occ.h>
}

#ifndef OCCGEOMETRY
#define OCCGEOMETRY
#endif
#include <meshing.hpp>
#include <occgeom.hpp>

namespace {
inline void NOOP_Deleter(void*) { ; }
}

namespace netgen {
NETGENPLUGIN_DLL_HEADER
extern MeshingParameters mparam;
}

//----------------------------------------------------------------------------
NetgenPlugin_NetgenLibWrapper::NetgenPlugin_NetgenLibWrapper()
	: _ngMesh(nullptr) {
	if (instanceCounter() == 0) {
		nglib::Ng_Init();

		netgen::testout = new std::ofstream("test.out");
		ngcore::Logger::SetGlobalLoggingLevel(ngcore::level::trace);
		ngcore::printmessage_importance = 2;
		std::cout << "PRINT MSG IMP: " << ngcore::printmessage_importance << std::endl;

		// if (!netgen::testout)
		// 	netgen::testout = new std::ofstream("test.out");
	}
	++instanceCounter();

	_isComputeOk = false;
	_coutBuffer = nullptr;
	_ngcout = nullptr;
	_ngcerr = nullptr;

	// redirect all netgen output (mycout,myerr,cout) to _outputFileName
	_outputFileName = this->getOutputFileName();
	_ngcout = netgen::mycout;
	_ngcerr = netgen::myerr;
	// netgen::mycout = new std::ofstream(_outputFileName.c_str());
	// std::ofstream* outFile = dynamic_cast<std::ofstream*>(netgen::mycout);

	// if (outFile && !outFile->is_open()) {
	// 	std::cerr << "Failed to open the output file: " << _outputFileName << std::endl;
	// 	return;
	// }

	netgen::myerr = netgen::mycout;
	_coutBuffer = std::cout.rdbuf();

#ifdef _DEBUG_
	std::cout << "NOTE: netgen output is redirected to file " << _outputFileName << std::endl;
#else
	std::cout.rdbuf(netgen::mycout->rdbuf());
#endif
	this->setMesh(nglib::Ng_NewMesh());
}

//----------------------------------------------------------------------------
NetgenPlugin_NetgenLibWrapper::~NetgenPlugin_NetgenLibWrapper() {
	--instanceCounter();

	// FIXME: This causes segmentation fault error
	// nglib::Ng_DeleteMesh(this->ngMesh());
	nglib::Ng_Exit();
	if (_coutBuffer)
		std::cout.rdbuf(_coutBuffer);

	this->removeOutputFile();
}

//----------------------------------------------------------------------------
int& NetgenPlugin_NetgenLibWrapper::instanceCounter() {
	static int theCouner = 0;
	return theCouner;
}

//----------------------------------------------------------------------------
std::string NetgenPlugin_NetgenLibWrapper::getOutputFileName() {
	std::string tmpDir = std::filesystem::temp_directory_path().string();

	if (!tmpDir.empty() && tmpDir.back() != '/' && tmpDir.back() != '\\') {
		tmpDir += '/';
	}

	TCollection_AsciiString aGenericName = tmpDir.c_str();
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
void NetgenPlugin_NetgenLibWrapper::removeOutputFile() {
	if (!_outputFileName.empty()) {
		if (_ngcout) {
			// delete netgen::mycout;
			netgen::mycout = _ngcout;
			netgen::myerr = _ngcerr;
			_ngcout = 0;
		}
		// ToDo: Remove temporary files
	}
}

//----------------------------------------------------------------------------
void NetgenPlugin_NetgenLibWrapper::setMesh(nglib::Ng_Mesh* mesh) {
	if (_ngMesh)
		nglib::Ng_DeleteMesh(this->ngMesh());
	_ngMesh = (netgen::Mesh*)mesh;
}

//----------------------------------------------------------------------------
int NetgenPlugin_NetgenLibWrapper::GenerateMesh(
	netgen::OCCGeometry& occgeo, int startWith, int endWith, netgen::Mesh*& ngMesh) {
	int err = 0;
	if (!ngMesh)
		ngMesh = new netgen::Mesh;

	ngMesh->SetGeometry(std::shared_ptr<netgen::NetgenGeometry>(&occgeo, &NOOP_Deleter));

	netgen::mparam.perfstepsstart = startWith;
	netgen::mparam.perfstepsend = endWith;
	std::shared_ptr<netgen::Mesh> meshPtr(ngMesh, &NOOP_Deleter);
	err = occgeo.GenerateMesh(meshPtr, netgen::mparam);

	return err;
}

//----------------------------------------------------------------------------
void NetgenPlugin_NetgenLibWrapper::CalcLocalH(netgen::Mesh* ngMesh) {
	ngMesh->CalcLocalH(netgen::mparam.grading);
}
