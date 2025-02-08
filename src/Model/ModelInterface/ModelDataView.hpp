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

#ifndef MODELDATAVIEW_HPP
#define MODELDATAVIEW_HPP

#include "ModelManager.hpp"

class ModelDataView {

public:
	using EntityType = GeometryCore::EntityType;
	using ShapesTags = std::vector<int>;
	using ShapeRef = std::reference_wrapper<const TopoDS_Shape>;

	ModelDataView(const ModelManager& aModelManger);
	~ModelDataView() = default;

	const GeometryCore::PartsMap& getPartsMap() const;

	const TopoDS_Shape& getShape(const EntityType& aEntityType, int aShapeTag) const;
	const TopoDS_Shape& getShape(
		const EntityType& aEntityType, const std::string& aShapeName) const;

	const std::vector<ShapeRef> getShapes(
		const EntityType& aEntityType, const std::vector<int>& aShapesTags) const;
	const std::vector<ShapeRef> getShapes(
		const EntityType& aEntityType, const std::vector<std::string>& aShapesNames) const;

	int getShapeTag(const TopoDS_Shape&) const;
	std::string getShapeName(const TopoDS_Shape&) const;

	vtkSmartPointer<vtkActor> getMeshActor() const;

private:
	const ModelManager& _modelManager;
};

#endif