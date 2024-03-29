#include "GeometryImporter.h"

std::string GeometryCore::GeometryImporter::getUniqueObjectName(const std::string& prefix, const PartsMap& objectMap){
 	int i = 0;
    std::string uniqueName;
    for (const auto& pair : objectMap) {
        const std::string& stringObj = pair.first;

        if (stringObj.find(prefix) != std::string::npos) {
            std::stringstream stringStream;
            stringStream << prefix << std::setfill('0') << std::setw(3) << i;
            uniqueName = stringStream.str();
            if (objectMap.find(uniqueName) == objectMap.end()) {
                break;
            }
            i++;
        }
    }
	return uniqueName;
};


GeometryCore::ActorsMap GeometryCore::GeometryImporter::getPartsActorMap(){
	Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(this->_dataFrame->Main());
	Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_dataFrame->Main());

	GeometryCore::ActorsMap actorsMap {};

	for (const auto& it : this->_partsMap) {
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

GeometryCore::ActorsMap GeometryCore::GeometryImporter::getFacesActorMap(){
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

GeometryCore::ActorsMap GeometryCore::GeometryImporter::getEdgesActorMap(){
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
};
