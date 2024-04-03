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

void Model::addParts(GeometryCore::PartsMap partsMap) { 
    this->_partsMap = partsMap;
            for (const auto& it : this->_partsMap) {
                std::cout << it.first << std::endl;
                const auto& shape = it.second;
                const void *shape_ptr;
                shape_ptr = &shape;
                std::vector< std::pair<int, int>> outDimTags;
                gmsh::model::occ::importShapesNativePointer(shape_ptr, outDimTags);
            }
            std::cout << "Loaded " << _partsMap.size() << " parts into the model!" << std::endl;
            gmsh::model::occ::synchronize();
            meshParts();
}

void Model::meshParts() {
            gmsh::option::setNumber("Mesh.MeshSizeMin", 3);
            gmsh::option::setNumber("Mesh.MeshSizeMax", 3);
            gmsh::model::mesh::generate(3);
            gmsh::write(_modelName + ".msh");
            this->polyData = createMeshVtkPolyData();
            std::cout << "Parts Meshed!" << std::endl;
}

vtkSmartPointer<vtkPolyData> Model::createMeshVtkPolyData() {
    std::cout << "moving mesh to vtkPolyData..." << std::endl;

    // extract nodes from gmsh model, and transfer them to vtkPoints container
    // nodeTags is a vector containing node numbering (tag - hence the name)
    // nodeCoords holds coordinates of each node [node_1_x, node_1_y, node_1_z, node_2_x, etc...]

    std::vector<unsigned long long> nodeTags;
    std::vector<double> nodeCoords;
    std::vector<double> parametricCoord;

    gmsh::model::mesh::getNodes(nodeTags, nodeCoords, parametricCoord);

    vtkSmartPointer<vtkPoints> vtkNodes = vtkSmartPointer<vtkPoints>::New();
    int xNodeId, yNodeId, zNodeId;
    for(size_t i = 0; i < nodeTags.size(); ++i)
    {
        xNodeId = i*3;
        yNodeId = xNodeId + 1;
        zNodeId = xNodeId + 2;
        vtkNodes->InsertNextPoint(nodeCoords[xNodeId],
                                    nodeCoords[yNodeId],
                                    nodeCoords[zNodeId]);
    }


    // extract 3D elements from gmsh model, and transfer them to vtkCellArray container
    // elementTypes is a vector containing types of elemnts in the mesh
    // elementTags is a vevtor of length equal to elementTypes. It holds a vector of element
    // ids (tags) for each element type. Ex elementTags[0].size() would return a total number
    // of elements of type elementTags[0] (so far only tetras)
    // elementNodeTags is a vevtor of length equal to elementTypes. For each type it stores
    // a vector of node tags concatenated: [e1n1, e1n2, ..., e1nN, e2n1, ...]
    // in our case we so far handle only tetrahedral elements (so 4 nodes per ele)

    std::vector<int> _elementTypes;
    std::vector<ElementType> elementTypes;
    std::vector<std::vector<unsigned long long>> elementTags;
    std::vector<std::vector<unsigned long long>> elementNodeTags;

    gmsh::model::mesh::getElements(_elementTypes, elementTags, elementNodeTags, -1);
    vtkSmartPointer<vtkCellArray> vtkElements = vtkSmartPointer<vtkCellArray>::New();

    elementTypes.reserve(_elementTypes.size());
    std::transform(_elementTypes.begin(), _elementTypes.end(), std::back_inserter(elementTypes),
    [](int& elem){return static_cast<ElementType>(elem);});

    std::vector<vtkIdType> currentElementNodeTags;
    currentElementNodeTags.reserve(4);

    vtkSmartPointer<vtkCellArray> vtkLines = vtkSmartPointer<vtkCellArray>::New();
    std::vector<vtkIdType> currentLineNodeTags;
    currentLineNodeTags.reserve(2); // Assuming each line has two nodes

    for(size_t i = 0; i < elementTypes.size(); ++i)
    {
            if (elementTypes[i] == ElementType::TETRA)
            {
                for(size_t j = 0; j < elementTags[i].size(); ++j)
                {
                    auto startNodeIterator = std::make_move_iterator(elementNodeTags[i].begin() + j * 4);
                    auto endNodeIterator = std::make_move_iterator(elementNodeTags[i].begin() + (j + 1) * 4);

                    std::transform(startNodeIterator, endNodeIterator, std::back_inserter(currentElementNodeTags),
                    [](unsigned long long nodeId) { return static_cast<vtkIdType>(nodeId);});
                    vtkElements->InsertNextCell(4, currentElementNodeTags.data());
                    currentElementNodeTags.clear();
                }
            }else if (elementTypes[i] == ElementType::LINE) // Assuming ElementType::LINE is the type for lines
            {
                for(size_t j = 0; j < elementTags[i].size(); ++j)
                {
                    auto startNodeIterator = std::make_move_iterator(elementNodeTags[i].begin() + j * 2); // Assuming each line has two nodes
                    auto endNodeIterator = std::make_move_iterator(elementNodeTags[i].begin() + (j + 1) * 2);

                    std::transform(startNodeIterator, endNodeIterator, std::back_inserter(currentLineNodeTags),
                    [](unsigned long long nodeId) { return static_cast<vtkIdType>(nodeId);});
                    vtkLines->InsertNextCell(2, currentLineNodeTags.data());
                    currentLineNodeTags.clear();
                }
            }
    }

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

    polyData->SetPoints(vtkNodes);
    polyData->SetLines(vtkLines);
    polyData->SetPolys(vtkElements);
    
    std::cout << "Succesively created vtkPolyData object!" << std::endl;
    return polyData;
}


void Model::updateMeshActor()
{
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(this->polyData);
    this->meshActor = vtkSmartPointer<vtkActor>::New();
    this->meshActor->SetMapper(mapper);
};