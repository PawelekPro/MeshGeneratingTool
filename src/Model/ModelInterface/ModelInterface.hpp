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
* File      : ModelInterface.hpp
* Author    : Krystian Fudali, Paweł Gilewicz
* Date      : 28/01/2025
*/

#ifndef MODELINTERFACE_HPP
#define MODELINTERFACE_HPP

#include "ModelDataView.hpp"
#include "ModelManager.hpp"

class MGTMesh_ProxyMesh;
class vtkActor;

class ModelInterface {

public:
	ModelInterface(ModelManager& aModelManager);

	void createNewModel(const QString& aNewModelName);
    void addObserver(std::shared_ptr<EventObserver> aObserver);

	int importSTEP(const QString& aFilePath, QWidget* progressBar);
	int importSTL(const QString& aFilePath, QWidget* progressBar);

	bool generateMesh(bool surfaceMesh = false);

	const ModelDataView& modelDataView() { return _modelDataView; };

private:
	ModelManager& _modelManager;
	const ModelDataView _modelDataView;
};

#endif
