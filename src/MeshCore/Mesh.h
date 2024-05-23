#ifndef MESH_H
#define MESH_H

#include "MeshContainers.hpp"

#ifdef _WIN32
#include <gmsh.h_cwrap>
#endif

#ifdef linux
#include <gmsh.h>
#endif

#include <algorithm>
#include <vector>

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

namespace MeshCore{
    
    enum class ElementType : int {
        TETRA = 4,
        TRIANGLE = 2,
        LINE = 1,
        POINT = 15,
    };

    class Mesh{
        public:
            Mesh();
            vtkSmartPointer<vtkActor> getMeshActor(){return this->_meshActor;};
            void addSizing(std::vector<int> verticesTags, double size);
            void genereateSurfaceMesh();

        private:
            
            void fillElementDataMap(ElementType elementType);
            void fillVtkNodes();
            void updatePolyData();

            vtkSmartPointer<vtkActor> createMeshActor(vtkSmartPointer<vtkPolyData> polyData);


            std::vector<MeshSizing> _meshSizings;
            

            std::map<ElementType, vtkElementData> _elementDataMap;

            vtkSmartPointer<vtkPoints> _vtkNodes;
            vtkSmartPointer<vtkPolyData> _polyData;
            vtkSmartPointer<vtkActor> _meshActor;
    };
};


#endif