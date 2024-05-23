#ifndef MESH_H
#define MESH_H

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

    struct MeshSizing{
        
        MeshSizing(std::vector<int> verticesTags, double size);

        double elementSize;
        std::vector<std::pair<int, int>> nodeTags;
    };

    class Mesh{
        public:

            vtkSmartPointer<vtkActor> getMeshActor(){return this->_meshActor;};

            void addSizing(std::vector<int> verticesTags, double size);
            void genereateSurfaceMesh();

        private:
            
            vtkSmartPointer<vtkPolyData> createSurfaceMeshPolyData();

            vtkSmartPointer<vtkActor> createMeshActor(vtkSmartPointer<vtkPolyData> polyData);

            enum class ElementType : int {
                TETRA = 4,
                TRIANGLE = 2,
                LINE = 1,
                POINT = 15,
            };
            std::vector<MeshSizing> _meshSizings;
            
            vtkSmartPointer<vtkPolyData> _polyData;
            vtkSmartPointer<vtkActor> _meshActor;
    };





};


#endif