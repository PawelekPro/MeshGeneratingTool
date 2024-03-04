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
 */

#include "STEPPlugin.h"

STEPGeometryPlugin::STEPGeometryPlugin() {
	this->stepOperations = STEPPlugin::STEPPluginOperations {};
}

void STEPGeometryPlugin::load(const std::string& fileName, QWidget* parent) {
	this->stepOperations.load(fileName, parent);

	this->edgesPartsMap = this->stepOperations.getEdgesPartsMap();

	this->facesPartsMap = this->stepOperations.getEdgesPartsMap();

	this->edgesActorsMap = this->stepOperations.getVTKEdgesMap();

	this->facesActorsMap = this->stepOperations.getVTKFacesMap();
}

Geometry::ActorsMap STEPGeometryPlugin::getVTKActorsMap() {
	Geometry::ActorsMap mergedMap;
	mergedMap.insert(
		this->facesActorsMap.begin(), this->facesActorsMap.end());
	mergedMap.insert(
		this->edgesActorsMap.begin(), this->edgesActorsMap.end());

	return mergedMap;
}
