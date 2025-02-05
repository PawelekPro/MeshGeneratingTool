/*
 * Copyright (C) 2024 Krystian Fudali
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

#ifndef MODEL_HPP
#define MODEL_HPP


#include "Geometry.hpp"
#include "Mesh.h"
#include "DocumentHandler.hpp"
#include "ModelSubject.hpp"
#include <memory>
#include <vector>

#ifdef _WIN32
#include <gmsh.h_cwrap>
#endif

#ifdef linux
#include <gmsh.h>
#endif
class EventObserver;
class Model {
	public:
	static void initializeGmsh();

	std::string _modelName;
	GeometryCore::Geometry geometry;
	MeshCore::Mesh mesh;
	ModelSubject subject;

	Model(std::string modelName);
	~Model();

	Model(const Model& aOther) = delete;
	Model& operator=(const Model& aOther) = delete;

	void addObserver(std::shared_ptr<EventObserver> aObserver);

	//--------Geometry interface-----// 
    void importSTEP(const std::string& filePath, QWidget* progressBar);
    void importSTL(const std::string& filePath, QWidget* progressBar);

	//--------Meshing interface-----// 
	void meshSurface();
	void meshVolume();
	vtkSmartPointer<vtkActor> getMeshActor();

	void applyMeshSettings();

	void addSizing(const std::vector<std::reference_wrapper<const TopoDS_Shape>> selectedShapes);
	void addSizing(const std::vector<int>& verticesTags, double size);

	private:

	static bool gmshInitialized;

	void applyMeshGlobalSize();
	void applyMeshSizings();

	void addShapesToModel(const GeometryCore::PartsMap& shapesMap);
};

#endif
