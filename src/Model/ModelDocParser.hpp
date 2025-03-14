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

#ifndef MODELDOCPARSER_HPP
#define MODELDOCPARSER_HPP

#include "DocumentHandler.hpp"
#include "Model.hpp"

class MGTMesh_Algorithm;

class ModelDocParser {

public:
	ModelDocParser(Model& aModel);

	void applyMeshSettings();
	void applyElementSizings();

	std::pair<std::vector<int>, double> parseElementSizing(const QDomElement& aSizingElement);
	std::unique_ptr<MGTMesh_Algorithm> generateMeshAlgorithm(bool surfaceMesh = false) const;

private:
	Model& _model;
	const DocumentHandler& _doc;
};

#endif