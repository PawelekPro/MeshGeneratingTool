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
* File      : GMSHPlugin_Parameters.hpp
* Author    : Paweł Gilewicz
* Date      : 23/02/2025
*/

#ifndef GMSHPLUGIN_PARAMETERS_HPP
#define GMSHPLUGIN_PARAMETERS_HPP

#include "GMSHPlugin_Defs.hpp"
#include "MGTMesh_Algorithm.hpp"

class GMSHPLUGIN_EXPORT GMSHPlugin_Parameters : public MGTMesh_Algorithm {
public:
	GMSHPlugin_Parameters(int schemeId);
	~GMSHPlugin_Parameters() = default;

	enum Algo2D {
		automatic,
		meshadapt,
		delaunay,
		frontal,
		delaunayforquad,
		packingparallelograms,
		quadqs
	};

	void Set2DAlgo(Algo2D the2DAlgo);
	Algo2D Get2DAlgo() const { return _algo2d; }

	enum Algo3D { delaunay3, frontal3, mmg3d, rtree, hxt };

	void Set3DAlgo(Algo3D the3DAlgo);
	Algo3D Get3DAlgo() const { return _algo3d; }

	enum Recomb2DAlgo { simple, blossom, simplefullquads, blossomfullquads };

	void SetRecomb2DAlgo(Recomb2DAlgo theRecomb2DAlgo);
	Recomb2DAlgo GetRecomb2DAlgo() const { return _recomb2DAlgo; }

	enum SubdivAlgo { none, allquads, allhexas };

	void SetSubdivAlgo(SubdivAlgo theSubdivAlgo);
	SubdivAlgo GetSubdivAlgo() const { return _subdivAlgo; }

	enum RemeshAlgo { nosplit, automaticR, automaticmetis };

	void SetRemeshAlgo(RemeshAlgo theRemeshAlgo);
	RemeshAlgo GetRemeshAlgo() const { return _remeshAlgo; }

	enum RemeshPara { harmonic, conformal, rbfharmonic };

	void SetRemeshPara(RemeshPara theRemeshPara);
	RemeshPara GetRemeshPara() const { return _remeshPara; }

	void SetSmoothSteps(double smoothSteps);
	double GetSmoothSteps() const { return _smoothSteps; }

	void SetMeshCurvatureSize(double meshCurvaturesize);
	double GetMeshCurvatureSize() const { return _meshCurvatureSize; };

	void SetMeshSizeFactor(double meshSizeFactor);
	double GetMeshSizeFactor() const { return _sizeFactor; }

	void SetMeshSizeMin(double minSize);
	double GetMeshSizeMin() const { return _minSize; }

	void SetMeshSizeMax(double maxSize);
	double GetMeshSizeMax() const { return _maxSize; }

private:
	Algo2D _algo2d;
	Algo3D _algo3d;
	Recomb2DAlgo _recomb2DAlgo;
	bool _recombineAll;
	SubdivAlgo _subdivAlgo;
	RemeshAlgo _remeshAlgo;
	RemeshPara _remeshPara;
	double _smoothSteps;
	double _sizeFactor;
	double _meshCurvatureSize;

	double _minSize, _maxSize;
	bool _secondOrder, _useIncomplElem;
	bool _is2d;
};

#endif