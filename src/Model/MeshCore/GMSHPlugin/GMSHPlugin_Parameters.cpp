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
* File      : GMSHPlugin_Parameters.cpp
* Author    : Paweł Gilewicz
* Date      : 23/02/2025
*/

#include "GMSHPlugin_Parameters.hpp"

#include "MGTMesh_Algorithm.hpp"

//----------------------------------------------------------------------------
GMSHPlugin_Parameters::GMSHPlugin_Parameters(int schemeId)
	: MGTMesh_Algorithm(schemeId)
	, _algo2d(automatic)
	, _algo3d(delaunay3)
	, _recomb2DAlgo(simple)
	, _recombineAll(false)
	, _subdivAlgo(none)
	, _remeshAlgo(nosplit)
	, _remeshPara(harmonic)
	, _smoothSteps(1)
	, _sizeFactor(1)
	, _meshCurvatureSize(0)
	, _minSize(0)
	, _maxSize(1e22)
	, _secondOrder(false)
	, _useIncomplElem(true) { }

//----------------------------------------------------------------------------
void GMSHPlugin_Parameters::Set2DAlgo(Algo2D the2DAlgo) {
	if (the2DAlgo != _algo2d) {
		_algo2d = the2DAlgo;
		// ToDo: NotifySubMeshesHypothesisModification();
	}
}

//----------------------------------------------------------------------------
void GMSHPlugin_Parameters::Set3DAlgo(Algo3D the3DAlgo) {
	if (the3DAlgo != _algo3d) {
		_algo3d = the3DAlgo;
		// ToDo: NotifySubMeshesHypothesisModification();
	}
}

//----------------------------------------------------------------------------
void GMSHPlugin_Parameters::SetRecomb2DAlgo(Recomb2DAlgo theRecomb2DAlgo) {
	if (theRecomb2DAlgo != _recomb2DAlgo) {
		_recomb2DAlgo = theRecomb2DAlgo;
		// ToDo: NotifySubMeshesHypothesisModification();
	}
}

//----------------------------------------------------------------------------
void GMSHPlugin_Parameters::SetSubdivAlgo(SubdivAlgo theSubdivAlgo) {
	if (theSubdivAlgo != _subdivAlgo) {
		_subdivAlgo = theSubdivAlgo;
		// ToDo: NotifySubMeshesHypothesisModification();
	}
}

//----------------------------------------------------------------------------
void GMSHPlugin_Parameters::SetRemeshAlgo(RemeshAlgo theRemeshAlgo) {
	if (theRemeshAlgo != _remeshAlgo) {
		_remeshAlgo = theRemeshAlgo;
		// ToDo: NotifySubMeshesHypothesisModification();
	}
}
//----------------------------------------------------------------------------
void GMSHPlugin_Parameters::SetRemeshPara(RemeshPara theRemeshPara) {
	if (theRemeshPara != _remeshPara) {
		_remeshPara = theRemeshPara;
		// ToDo: NotifySubMeshesHypothesisModification();
	}
}

//----------------------------------------------------------------------------
void GMSHPlugin_Parameters::SetSmoothSteps(double smoothSteps) {
	if (smoothSteps != _smoothSteps) {
		_smoothSteps = smoothSteps;
		// ToDo: NotifySubMeshesHypothesisModification();
	}
}