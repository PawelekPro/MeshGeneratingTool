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

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkIdTypeArray.h>


namespace MeshCore{

    class Mesh{
        public:

            Mesh(){};
            ~Mesh(){};

            vtkSmartPointer<vtkActor> getMeshActor(){return this->_meshActor;};

            void addSizing(std::vector<int> nodeToSizeTags);
            void genereateSurfaceMesh();
            void generateVolumeMesh();

        private:
            
            void updateMeshActor();

            enum class ElementType : int {
                TETRA = 4,
                TRIANGLE = 2,
                LINE = 1,
                POINT = 15,
            };

            vtkSmartPointer<vtkActor> _meshActor;

            std::vector<std::pair<int, int>> _sizingTags;
            
            vtkSmartPointer<vtkUnstructuredGrid> _gridData;
            vtkSmartPointer<vtkPolyData> _polyData;

    };
};


#endif