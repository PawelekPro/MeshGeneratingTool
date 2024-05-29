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



#include "Model.h"


Model::Model(std::string modelName) : _modelName(modelName) {
        gmsh::initialize();
        gmsh::model::add(_modelName);
        this->geometry = GeometryCore::Geometry();
        this->mesh = MeshCore::Mesh();
        };
Model::~Model(){
    gmsh::finalize();
}

void Model::addShapesToModel(const GeometryCore::PartsMap& shapesMap) { 
    for (const auto& it : shapesMap) {
        const auto& shape = it.second;
        const void *shape_ptr = &shape;
        std::vector< std::pair<int, int>> outDimTags;
        gmsh::model::occ::importShapesNativePointer(shape_ptr, outDimTags);
    }
    gmsh::model::occ::synchronize();
}

void Model::addSizing(std::vector<std::reference_wrapper<const TopoDS_Shape>> selectedShapes){
    for(const auto& shape : selectedShapes){
        const TopoDS_Shape& shapeRef =  shape.get();
        std::vector<int> vertexTags = geometry.getShapeVerticesTags(shape);
        mesh.addSizing(vertexTags, 0.1);
    }
}

void Model::meshSurface() {
    mesh.generateSurfaceMesh();
}
void Model::meshVolume(){
    mesh.generateVolumeMesh();
}
            
vtkSmartPointer<vtkActor> Model::getMeshActor(){
    return mesh.getMeshActor();
}

void Model::importSTL(const std::string& filePath, QWidget* progressBar){
    geometry.importSTL(filePath, progressBar);
    GeometryCore::PartsMap shapesMap = geometry.getShapesMap();
    addShapesToModel(shapesMap);
}

void Model::importSTEP(const std::string& filePath, QWidget* progressBar){
    geometry.importSTEP(filePath, progressBar);
    GeometryCore::PartsMap shapesMap = geometry.getShapesMap();
    addShapesToModel(shapesMap);
}