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
* File      : MGTMesh_Scheme.h
* Author    : Paweł Gilewicz
* Date      : 27/11/2024
*/
#ifndef MGTMESH_SCHEME_H
#define MGTMESH_SCHEME_H

#include <string>

class MGTMesh_Scheme {
public:
	enum Engine { NETGEN, GMSH };

	enum SchemeType { PARAM_ALG, ALG_0D, ALG_1D, ALG_2D, ALG_3D };

	enum SchemeStatus //! in the order of severity
	{
		OK = 0,
		MISSING, //! algo misses a scheme
		CONCURRENT, //! several applicable schemes assigned to shapes
		BAD_PARAMETER, //! scheme has a bad parameter value
		UNKNOWN_FATAL, //!  all statuses below should be considered as fatal
		INCOMPATIBLE, //! scheme does not fit algo
		NOTCONFORM, //! not conform mesh is produced applying a scheme
		ALREADY_EXIST, //! several applicable schemes of same priority assigned
		BAD_DIM, //! bad dimension
		BAD_SUBSHAPE, //! shape is neither the main one, nor its sub-shape, nor a group
		BAD_GEOMETRY, //! shape geometry mismatches algorithm's expectation
		NEED_SHAPE, //! algorithm can work on shape only
		INCOMPAT_SCHEMES //! schemes are incompatible
	};

	static bool IsStatusFatal(const SchemeStatus status) { return status >= UNKNOWN_FATAL; }

public:
	explicit MGTMesh_Scheme(int schemeId);
	virtual ~MGTMesh_Scheme() = default;

public:
	[[nodiscard]] const char* GetName() const;

	[[nodiscard]] bool Is3DAlgortihm() const;
	[[nodiscard]] bool Is2DAlgortihm() const;
	[[nodiscard]] bool Is1DAlgortihm() const;

	[[nodiscard]] int GetID() const;
	[[nodiscard]] int GetType() const;
	[[nodiscard]] int GetDim() const;
	[[nodiscard]] int GetShapeType() const;
	[[nodiscard]] int GetEngineLib() const;

	void SetType(SchemeType type);
	void SetDim(int algDim);
	void SetShapeType(int shapeType);
	void SetEngineLib(MGTMesh_Scheme::Engine libType);

protected:
	std::string _name; //! identifier of scheme type
	int _schemeId; //! ID unique within application session
	SchemeType _type; //! enum SchemeType
	int _shapeType;
	int _alg_dim;

private:
	Engine _engineLib; //! type of library of plug-in engine
};

#endif
