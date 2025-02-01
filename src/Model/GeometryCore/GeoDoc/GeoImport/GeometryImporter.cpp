#include "GeometryImporter.hpp"

std::string GeometryCore::GeometryImporter::getUniqueObjectName(const std::string& prefix, const PartsMap& objectMap){
 	int i = 1;
    std::string uniqueName;
    while (true) {
        std::stringstream stringStream;
        stringStream << prefix << std::setfill('0') << std::setw(3) << i;
        uniqueName = stringStream.str();
        
        if (objectMap.find(uniqueName) == objectMap.end()) {
            break;
        }
        i++; 
    }
    return uniqueName;
};

vtkSmartPointer<vtkActor> GeometryCore::GeometryImporter::createVTKActor(const TopoDS_Shape& shape){
	IVtkOCC_Shape* vtkShapeAdapter = new IVtkOCC_Shape(shape);
	auto dataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
	dataSource->SetShape(vtkShapeAdapter);

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(dataSource->GetOutputPort());
	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	return actor;
}