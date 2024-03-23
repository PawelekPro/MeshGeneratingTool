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

#ifndef MODEL_H
#define MODEL_H
#include "GeometryFunctions.h"

#include <gmsh.h_cwrap>
#include <TopoDS_Shape.hxx>

#include <algorithm>
#include <vector>

#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>


class Model
{
    public:
        std::string _modelName;
        

        Model(std::string modelName);
        ~Model();

        void addParts(Geometry::PartsMap partsMap);
        void meshParts();
        void updateMeshActor();


        vtkSmartPointer<vtkPolyData> createMeshVtkPolyData();


        vtkSmartPointer<vtkActor> meshActor;

    private:
        enum class ElementType : int{
        TETRA = 4,
        LINE = 1,
        };

    Geometry::PartsMap _partsMap;
    vtkSmartPointer<vtkPolyData> polyData;

};
// some code

#endif //MODEL_H

