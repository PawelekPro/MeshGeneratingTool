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

#include "ModelDataView.hpp"

using ShapeRef = std::reference_wrapper<const TopoDS_Shape>;

ModelDataView::ModelDataView(const ModelManager& aModelManager)
	: _modelManager(aModelManager) {};

const GeometryCore::PartsMap& ModelDataView::getPartsMap() const {
	const Model& model = _modelManager.getModel();
	const GeometryCore::PartsMap& partsMap = model.geometry.getShapesMap();
	return partsMap;
};

const TopoDS_Shape& ModelDataView::getShape(const EntityType& aEntityType, int aShapeTag) const {
	const Model& model = _modelManager.getModel();
	const GeometryCore::TagMap& tagMap = model.geometry.getTagMap();
	switch (aEntityType) {
	case (EntityType::Vertex): {
		return tagMap.getShape(EntityType::Vertex, aShapeTag);
		break;
	}
	case (EntityType::Edge): {
		return tagMap.getShape(EntityType::Edge, aShapeTag);
		break;
	}
	case (EntityType::Face): {
		return tagMap.getShape(EntityType::Face, aShapeTag);
		break;
	}
	case (EntityType::Solid): {
		return tagMap.getShape(EntityType::Solid, aShapeTag);
		break;
	}
	}
	return TopoDS_Shape();
};

const TopoDS_Shape& ModelDataView::getShape(const EntityType& aEntityType, const std::string& aShapeName) const {
	return TopoDS_Shape();
};

const std::vector<ShapeRef> ModelDataView::getShapes(
	const EntityType& aEntityType,
	const std::vector<int>& aShapesTags) const {
	const Model& model = _modelManager.getModel();
	const GeometryCore::TagMap& tagMap = model.geometry.getTagMap();

	std::vector<ShapeRef> vec;
	vec.reserve(aShapesTags.size());

	std::transform(
		aShapesTags.begin(),
		aShapesTags.end(),
		std::back_inserter(vec),
		[&tagMap, aEntityType](int tag) -> ShapeRef {
			return std::cref(tagMap.getShape(aEntityType, tag));
		});
	return vec;
}

const std::vector<ShapeRef> ModelDataView::getShapes(const EntityType& aEntityType, const std::vector<std::string>& aShapesNames) const {
	std::vector<ShapeRef> vec;
	return vec;
};

int ModelDataView::getShapeTag(const TopoDS_Shape& aShape) const {
	const Model& model = _modelManager.getModel();
	const GeometryCore::TagMap& tagMap = model.geometry.getTagMap();
	int tag = tagMap.getTag(aShape);
	return tag;
};

std::string ModelDataView::getShapeName(const TopoDS_Shape&) const {
	return "notImplementedYet";
};

vtkSmartPointer<vtkActor> ModelDataView::getMeshActor() const {
	const Model& model = _modelManager.getModel();
	// vtkSmartPointer<vtkActor> meshActor = model.mesh.getMeshActor();
	// return meshActor;
}