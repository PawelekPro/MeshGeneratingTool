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
#include <vtkUnstructuredGrid.h>

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
            void addSizing(std::vector<int> verticesTags, double size);
            void generateSurfaceMesh();
            void generateVolumeMesh();

            vtkSmartPointer<vtkPoints> getVtkPoints();
            vtkSmartPointer<vtkCellArray> getVtkCellArray(ElementType ElementType);
            vtkSmartPointer<vtkActor> getMeshActor(){return this->_meshActor;};

        private:

            void fillVtkPoints();
            void fillElementDataMap(ElementType elementType);

            vtkSmartPointer<vtkActor> createMeshActor(vtkSmartPointer<vtkPolyData> polyData);
            std::vector<MeshSizing> _meshSizings;
            std::map<ElementType, vtkElementData> _elementDataMap;

            vtkSmartPointer<vtkPoints> _vtkPoints;
            vtkSmartPointer<vtkPolyData> _polyData;
            vtkSmartPointer<vtkUnstructuredGrid> _gridData;
            vtkSmartPointer<vtkActor> _meshActor;
    };
};
#endif