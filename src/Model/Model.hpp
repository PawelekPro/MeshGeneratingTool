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
* File      : Model.hpp
* Author    : Paweł Gilewicz, Krystian Fudali
* Date      : 26/01/2025
*/

#ifndef MODEL_HPP
#define MODEL_HPP

#include "DocumentHandler.hpp"
#include "Geometry.hpp"

#include <unordered_map>

class MGTMesh_Algorithm;
class MGTMesh_MeshObject;
class MGTMesh_ProxyMesh;

class Model {
public:
	std::string _modelName;
	GeometryCore::Geometry geometry;
	// MeshCore::Mesh mesh;

	explicit Model(std::string modelName);
	~Model();

	Model(const Model& aOther) = delete;
	Model& operator=(const Model& aOther) = delete;

	//--------Geometry interface-----//
	void importSTEP(const std::string& filePath, QWidget* progressBar);
	void importSTL(const std::string& filePath, QWidget* progressBar);

	//--------Meshing interface-----//
	bool generateMesh(const MGTMesh_Algorithm* algorithm);
	MGTMesh_ProxyMesh* getProxyMesh() const;

private:
	void addShapesToModel(const GeometryCore::PartsMap& shapesMap);

private:
	GeometryCore::PartsMap _shapesMap;

	std::unordered_map<int, vtkSmartPointer<MGTMesh_MeshObject>> _meshObjectsMap;
	std::shared_ptr<MGTMesh_ProxyMesh> _proxyMesh;
};

#endif
