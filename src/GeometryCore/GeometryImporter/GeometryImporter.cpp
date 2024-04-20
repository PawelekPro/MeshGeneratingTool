#include "GeometryImporter.h"

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


const GeometryCore::ActorsMap GeometryCore::GeometryImporter::getPartsActorMap(){
	Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(this->_dataFrame->Main());
	Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_dataFrame->Main());

	GeometryCore::ActorsMap actorsMap {};

	for (const auto& it : this->_solidsMap) {
		const auto& shape = it.second;

		vtkSmartPointer<vtkActor> actor = createVTKActor(shape);

		auto color = Quantity_Color {};
		colorTool->GetColor(shape, XCAFDoc_ColorGen, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorGen, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorSurf, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorCurv, color);

		actor->GetProperty()->SetColor(color.Red(), color.Green(), color.Blue());
		// std::stringstream stringStream;
		// stringStream << std::addressof(*actor.GetPointer());
		// std::string actorKey = stringStream.str();

		actorsMap[it.first] = actor;
	}

	return actorsMap;
};

const GeometryCore::ActorsMap GeometryCore::GeometryImporter::getFacesActorMap(){
	GeometryCore::ActorsMap facesMap {};
	for (const auto& it : this->_facesMap) {
		const auto& shape = it.second;

		vtkSmartPointer<vtkActor> actor = createVTKActor(shape);

		// actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
		actor->GetProperty()->SetLineWidth(3);

		// std::stringstream stringStream;
		// stringStream << std::addressof(* actor.GetPointer());
		// std::string actorKey = stringStream.str();

		facesMap[it.first] = actor;
	}
	return facesMap;
};

const GeometryCore::ActorsMap GeometryCore::GeometryImporter::getEdgesActorMap(){
	GeometryCore::ActorsMap edgesMap {};
	for (const auto& it : this->_edgesMap) {
		const auto& shape = it.second;

		vtkSmartPointer<vtkActor> actor = createVTKActor(shape);

		// actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
		actor->GetProperty()->SetLineWidth(3);

		// std::stringstream stringStream;
		// stringStream << std::addressof(*actor.GetPointer());
		// std::string actorKey = stringStream.str();

		edgesMap[it.first] = actor;
	}
	return edgesMap;
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