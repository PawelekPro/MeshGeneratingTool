#ifndef MESHCONTAINERS_H
#define MESHCONTAINERS_H


#include <vector>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCellArray.h>

namespace MeshCore{

    struct MeshSizing{
        
        MeshSizing(std::vector<int> verticesTags, double size);

        double elementSize;
        std::vector<std::pair<int, int>> nodeTags;
    };

    struct vtkElementData {
        vtkElementData() 
            : nCellNodes(0), 
            vtkCellType(0), 
            vtkCells(vtkSmartPointer<vtkCellArray>::New()),
            filled(false) {}

        vtkElementData(int nNodes, int vtkType)
            : nCellNodes(nNodes),
            vtkCellType(vtkType),
            vtkCells(vtkSmartPointer<vtkCellArray>::New()),
            filled(false) {}

    bool filled;
    int nCellNodes;
    vtkSmartPointer<vtkCellArray> vtkCells;
    int vtkCellType;
    };
}

#endif