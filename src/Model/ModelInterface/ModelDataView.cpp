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

ModelDataView::ModelDataView(const ModelManager& aModelManager) : _modelManager(aModelManager){};

const GeometryCore::PartsMap& ModelDataView::getPartsMap() const {
    const Model& model =  _modelManager.getModel();
    const GeometryCore::PartsMap& partsMap = model.geometry.getShapesMap();
    return partsMap;
};

const TopoDS_Shape& ModelDataView::getShape(int aShapeTag) const {
    return TopoDS_Shape();
};

const TopoDS_Shape& ModelDataView::getShape(const std::string& aShapeName) const {
    return TopoDS_Shape();
};

const std::vector<ShapeRef> ModelDataView::getShapes(const std::vector<int>& aShapesTags) const {
    std::vector<ShapeRef> vec;
    return vec;
};

const std::vector<ShapeRef> ModelDataView::getShapes(const std::vector<std::string>& aShapesNames) const {
    std::vector<ShapeRef> vec;
    return vec;
};

int ModelDataView::getShapeTag(const TopoDS_Shape&) const {
    return 0;
};

std::string ModelDataView::getShapeName(const TopoDS_Shape&) const {
    return "notImplementedYet";
};
