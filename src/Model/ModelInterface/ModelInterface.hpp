/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
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
	explicit ModelInterface(ModelManager& aModelManager);

	void createNewModel(const QString& aNewModelName) const;

	int importSTEP(const QString& aFilePath, QWidget* progressBar) const;
	int importSTL(const QString& aFilePath, QWidget* progressBar) const;

	bool generateMesh(bool surfaceMesh = false,
		const std::function<void(int)>& progressCallback = {},
		const std::function<void(const std::string&)>& statusCallback
		= {}) const;
	void CancelMeshGeneration() const;

	const ModelDataView& modelDataView() const { return _modelDataView; };

private:
	ModelManager& _modelManager;
	const ModelDataView _modelDataView;
};

#endif
