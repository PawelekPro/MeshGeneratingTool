#include "Mesh.h"

void MeshCore::Mesh::addSizing(std::vector<int> verticesTags, double size){
    MeshSizing meshSizing(verticesTags, size);
    _meshSizings.push_back(meshSizing);
};

void MeshCore::Mesh::genereateSurfaceMesh(){
    gmsh::option::setNumber("Mesh.MeshSizeMin", 0.1);
    gmsh::option::setNumber("Mesh.MeshSizeMax", 1);

    for(const auto& meshSizing : _meshSizings){
        gmsh::model::mesh::setSize(meshSizing.nodeTags,
                                   meshSizing.elementSize);
    }
    gmsh::model::mesh::generate(2);
    this->_polyData = createSurfaceMeshPolyData();
    this->_meshActor = createMeshActor(_polyData);
}

vtkSmartPointer<vtkActor> MeshCore::Mesh::createMeshActor(vtkSmartPointer<vtkPolyData> polyData){
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(_polyData);
        vtkSmartPointer<vtkActor> meshActor = vtkSmartPointer<vtkActor>::New();
        meshActor->SetMapper(mapper);
        meshActor->GetProperty()->EdgeVisibilityOn();
        meshActor->GetProperty()->SetEdgeColor(0.0, 0.0, 0.0);
        return meshActor;
        }
        
vtkSmartPointer<vtkPolyData> MeshCore::Mesh::createSurfaceMeshPolyData(){
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
        }
    }
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(vtkNodes);
    polyData->SetLines(elementDataMap.at(ElementType::LINE).vtkCells);
    polyData->SetPolys(elementDataMap.at(ElementType::TRIANGLE).vtkCells);
    return polyData;
}

MeshCore::MeshSizing::MeshSizing(std::vector<int> verticesTags, double size){
    elementSize = size;
    for(auto vertexTag : verticesTags){
                std::pair<int, int> tagPair(0, vertexTag);
                nodeTags.push_back(tagPair);
            }
};