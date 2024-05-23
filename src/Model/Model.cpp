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
        gmsh::option::setNumber("General.Terminal", 1);
        this->geometry = GeometryCore::Geometry();
        this->mesh = MeshCore::Mesh();
        };
Model::~Model(){
    gmsh::finalize();
}

void Model::updateParts() { 
    GeometryCore::PartsMap partsMap = this->geometry.getShapesMap();
    for (const auto& it : partsMap) {
        const auto& shape = it.second;
        const void *shape_ptr = &shape;
        std::vector< std::pair<int, int>> outDimTags;
        gmsh::model::occ::importShapesNativePointer(shape_ptr, outDimTags);
    }
    std::cout << "Loaded " << partsMap.size() << " parts into the model!" << std::endl;
    gmsh::model::occ::synchronize();
}

void Model::meshParts() {
            gmsh::option::setNumber("Mesh.MeshSizeMin", 0.05);
            gmsh::option::setNumber("Mesh.MeshSizeMax", 2);
            std::cout<<"sizing entities:" << std::endl;
            for(auto sizingTag : this->_sizingTags){
                std::cout << "Dim: " << sizingTag.first << "Tag: " << sizingTag.second << std::endl;
            }
            std::vector<std::pair<int,int>> dimTags;
            gmsh::model::occ::getEntities(dimTags);
            std::cout<<"gmsh occ entities:" << std::endl;
            for(auto dimTag : dimTags){
                std::cout << "Dim: " << dimTag.first << "Tag: " << dimTag.second << std::endl;
            } 
            gmsh::model::mesh::setSize(this->_sizingTags, 0.05);
            std::cout << "Meshing..." << std::endl;
            gmsh::model::mesh::generate(3);
            // gmsh::write(_modelName + ".msh");
            this->polyData = createMeshVtkPolyData();
            std::cout << "Parts Meshed!" << std::endl;
}

vtkSmartPointer<vtkPolyData> Model::createMeshVtkPolyData() {
    std::cout << "moving mesh to vtkPolyData..." << std::endl;

    // extract nodes from gmsh model, and transfer them to vtkPoints container
    // nodeTags is a vector containing node numbering (tag - hence the name)
    // nodeCoords holds coordinates of each node [node_1_x, node_1_y, node_1_z, node_2_x, etc...]
    #ifdef _WIN32
    std::vector<unsigned long long> nodeTags;
    #endif
    #ifdef linux
    std::vector<unsigned long> nodeTags;
    #endif
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

    #ifdef _WIN32
    std::vector<std::vector<unsigned long long>> elementTags;
    std::vector<std::vector<unsigned long long>> elementNodeTags;
    #endif
    #ifdef linux
    std::vector<std::vector<unsigned long>> elementTags;
    std::vector<std::vector<unsigned long>> elementNodeTags;
    #endif

    gmsh::model::mesh::getElements(_elementTypes, elementTags, elementNodeTags, -1);

    elementTypes.reserve(_elementTypes.size());
    std::transform(_elementTypes.begin(), _elementTypes.end(), std::back_inserter(elementTypes),
    [](int& elem){return static_cast<ElementType>(elem);});

    struct ElementData {
        ElementData() 
            : nCellNodes(0), 
            vtkCellType(0), 
            vtkCells(vtkSmartPointer<vtkCellArray>::New()) {}

        ElementData(int nNodes, int vtkType)
            : nCellNodes(nNodes),
            vtkCellType(vtkType),
            vtkCells(vtkSmartPointer<vtkCellArray>::New()) {}

    int nCellNodes;
    vtkSmartPointer<vtkCellArray> vtkCells;
    int vtkCellType;
    };

    std::map<ElementType, ElementData> elementDataMap = {
        {ElementType::TETRA, {4, VTK_TETRA}},
        {ElementType::TRIANGLE, {3, VTK_TRIANGLE}},
        {ElementType::LINE, {2, VTK_LINE}}
    };

    this->gridData = vtkSmartPointer<vtkUnstructuredGrid>::New();
    this->gridData->SetPoints(vtkNodes);
    std::vector<vtkIdType> currentElementNodeTags;
    for(size_t i = 0; i < elementTypes.size(); ++i)
    {
        if(elementTypes[i]!=ElementType::POINT){
        ElementData elementData = elementDataMap.at(elementTypes[i]);
        auto nElements = elementTags[i].size();
        auto nNodes = elementData.nCellNodes;
        auto cellArray = elementData.vtkCells;
        auto cellType = elementData.vtkCellType;

        for(size_t elementId = 0; elementId < nElements; ++elementId){
            auto startNodeIterator = std::make_move_iterator(elementNodeTags[i].begin() + elementId * nNodes);
            auto endNodeIterator = std::make_move_iterator(elementNodeTags[i].begin() + (elementId + 1) * nNodes);

            std::transform(startNodeIterator, endNodeIterator, std::back_inserter(currentElementNodeTags),
                [](auto nodeId) { return static_cast<vtkIdType>(nodeId - 1);});
            cellArray->InsertNextCell(nNodes, currentElementNodeTags.data());
            currentElementNodeTags.clear(); 
        }
        gridData->SetCells(cellType, cellArray);
        }
    }
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(vtkNodes);
    polyData->SetLines(elementDataMap.at(ElementType::LINE).vtkCells);
    polyData->SetPolys(elementDataMap.at(ElementType::TRIANGLE).vtkCells);
    std::cout << "Succesfully created vtkPolyData object!" << std::endl;
    return polyData;
}


void Model::updateMeshActor()
{
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(this->polyData);
    this->meshActor = vtkSmartPointer<vtkActor>::New();
    this->meshActor->SetMapper(mapper);
    this->meshActor->GetProperty()->EdgeVisibilityOn();
    this->meshActor->GetProperty()->SetEdgeColor(0.0, 0.0, 0.0);
}

void Model::addSizing(std::vector<std::reference_wrapper<const TopoDS_Shape>> selectedShapes){
    
    TopoDS_Face face;
    std::vector<int> vertexTags;
    int vertex_tag;
    int face_tag;

    for (const auto& shapeRef : selectedShapes) {
        const TopoDS_Shape& shape = shapeRef.get();

        // Ensure shape is a face
        if (shape.ShapeType() != TopAbs_FACE) {
            std::cerr << "Shape is not a face." << std::endl;
            continue;
        }

        // Get the tag for the face
        face_tag = this->geometry._tagMap.getTag(TopoDS::Face(shape));
        std::cout << "Face tag: " << face_tag << std::endl;

        // Initialize shapeExplorer with the face
        TopExp_Explorer shapeExplorer(shape, TopAbs_VERTEX);
        for(; shapeExplorer.More(); shapeExplorer.Next()) {
            const TopoDS_Vertex& vertex = TopoDS::Vertex(shapeExplorer.Current());
            vertex_tag = this->geometry._tagMap.getTag(vertex);
            vertexTags.push_back(vertex_tag);
            std::cout << "Vertex tag: " << vertex_tag << std::endl;
        }
    }
    std::sort(vertexTags.begin(), vertexTags.end());
    vertexTags.erase(std::unique(vertexTags.begin(), vertexTags.end()), vertexTags.end());

    std::vector<std::pair<int, int>> sizingTags;
    for(auto vertexTag : vertexTags){
        std::pair<int, int> tagPair(0, vertexTag);
        // cout << "vertexTag: " << tagPair.first << " dim: " << tagPair.second << std::endl;
        sizingTags.push_back(tagPair);
    }
    this->_sizingTags = sizingTags;
    std::cout<<"Imposing sizing on shapes!" << std::endl;
}