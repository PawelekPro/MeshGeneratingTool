#include <filesystem>

#include <vtkLogger.h>
#include <vtkProperty.h>

#include <XCAFApp_Application.hxx>

#include "STEPFileReader.h"
#include <BRep_Builder.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <TDF_Attribute.hxx>
#include <TDataStd_Name.hxx>
#include <TopAbs.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <vtkPolyDataMapper.h>

void Importing::STEPFileReader::load(const std::string& fileName) {
	if (!std::filesystem::exists(fileName)) {
		auto message = "File " + fileName + " can not be found.";
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::no_such_file_or_directory);
		throw std::filesystem::filesystem_error(message, errorCode);
	}
	auto baseName = std::filesystem::path { fileName }.stem().generic_string();
	vtkLogF(INFO, "Geometry file path: \"%s\"", fileName.c_str());

	this->dataFramework = Handle(TDocStd_Document) {};
	auto app = XCAFApp_Application::GetApplication();
	app->NewDocument("MDTV-XCAF", this->dataFramework);
	STEPCAFControl_Reader cafReader {};

	// Reading colors mode
	cafReader.SetColorMode(true);
	// Reading layers information mode
	cafReader.SetLayerMode(true);
	// Reading names from step file mode
	cafReader.SetNameMode(true);

	IFSelect_ReturnStatus result = cafReader.ReadFile(fileName.c_str());
	if (result != IFSelect_RetDone) {
		auto message = "Error while reading file:" + fileName;
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		throw std::filesystem::filesystem_error(message, errorCode);
	}

	if (!cafReader.Transfer(this->dataFramework)) {
		auto message = "Error while reading file:" + fileName;
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		throw std::filesystem::filesystem_error(message, errorCode);
	}

	vtkLogF(INFO, "STEPCAFControl_Reader transferred successfully.");

	auto& reader = cafReader.Reader();
	this->partsMap = Importing::PartsMap {};
	auto shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->dataFramework->Main());

	Standard_Integer numberOfShapes = reader.NbShapes();
	if (numberOfShapes == 0) {
		auto message = "No shapes found in given STEP file.";
		vtkLogF(ERROR, message);
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		throw std::filesystem::filesystem_error(message, errorCode);
	} else {
		for (auto i = 1; i <= numberOfShapes; i++) {
			auto _shape = reader.Shape(i);

			// Load each solid as an own object
			auto explorer = TopExp_Explorer {};
			for (explorer.Init(_shape, TopAbs_SOLID); explorer.More(); explorer.Next()) {

				// Get the shape
				auto& solid = TopoDS::Solid(explorer.Current());

				auto uniqueName = getUniqueObjectName("SolidPart", this->partsMap);

				// Do we need to collect labels? (Solid/Shell)
				// std::stringstream stringStream;
				// auto label = shapeTool->FindShape(solid);

				// if (!label.IsNull()) {
				// 	Handle(TDF_Attribute) attribute;
				// 	if (label.FindAttribute(TDataStd_Name::GetID(), attribute)) {
				// 		stringStream << Handle(TDataStd_Name)::DownCast(attribute)->Get();
				// 	}
				// }

				// auto message = "STEP: Transferring Shape " + stringStream.str();
				// vtkLogF(INFO, message.c_str());
				// if (stringStream.str().size() == 0) {
				// 	stringStream << "Solid";
				// }
				this->partsMap[uniqueName] = solid;
				// std::cout << uniqueName.c_str() << std::endl;
			}

			// Load each non-solid part
			for (explorer.Init(_shape, TopAbs_SHELL, TopAbs_SOLID); explorer.More(); explorer.Next()) {
				// Get the shape
				auto _shell = TopoDS::Shell(explorer.Current());

				auto uniqueName = getUniqueObjectName("ShellPart", this->partsMap);

				// Do we need to collect labels? (Solid/Shell)
				// std::stringstream stringStream;
				// auto label = shapeTool->FindShape(_shell);
				// if (!label.IsNull()) {
				// 	Handle(TDF_Attribute) attribute;
				// 	if (label.FindAttribute(TDataStd_Name::GetID(), attribute)) {
				// 		stringStream << Handle(TDataStd_Name)::DownCast(attribute)->Get();
				// 	}
				// }

				// if (stringStream.str().size() == 0) {
				// 	stringStream << "Shell";
				// }
				this->partsMap[uniqueName] = _shell;
			}

			// Put all other components into a single compound
			Standard_Boolean emptyComp = Standard_True;
			auto builder = BRep_Builder {};
			TopoDS_Compound compound;
			builder.MakeCompound(compound);
			for (explorer.Init(_shape, TopAbs_FACE, TopAbs_SHELL); explorer.More(); explorer.Next()) {
				if (!explorer.Current().IsNull()) {
					builder.Add(compound, explorer.Current());
					emptyComp = Standard_False;
				}
			}
			for (explorer.Init(_shape, TopAbs_WIRE, TopAbs_FACE); explorer.More(); explorer.Next()) {
				if (!explorer.Current().IsNull()) {
					builder.Add(compound, explorer.Current());
					emptyComp = Standard_False;
				}
			}
			for (explorer.Init(_shape, TopAbs_EDGE, TopAbs_WIRE); explorer.More(); explorer.Next()) {
				if (!explorer.Current().IsNull()) {
					builder.Add(compound, explorer.Current());
					emptyComp = Standard_False;
				}
			}
			for (explorer.Init(_shape, TopAbs_VERTEX, TopAbs_EDGE); explorer.More(); explorer.Next()) {
				if (!explorer.Current().IsNull()) {
					builder.Add(compound, explorer.Current());
					emptyComp = Standard_False;
				}
			}

			if (!emptyComp) {
				vtkLogF(INFO, "Loaded all other free-flying shapes into a single compound.");
				// TODO: Do we need to collect labels? (Solid/Shell)
				std::string uniqueName = getUniqueObjectName("CompoundPart", this->partsMap);
				this->partsMap[uniqueName] = compound;
			}
		}
	}

	std::filesystem::path filePath(fileName);
	std::filesystem::path stepName = filePath.filename();
	auto message = "STEP file: " + stepName.string() + " loaded successfully.";
	vtkLogF(INFO, message.c_str());
}

Importing::ActorsMap Importing::STEPFileReader::getVTKActorsMap() {
	Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(this->dataFramework->Main());
	Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->dataFramework->Main());

	Importing::ActorsMap actorsMap {};

	for (const auto& it : this->partsMap) {
		const auto& shape = it.second;

		vtkSmartPointer<vtkActor> actor = createVTKActor(shape);

		auto color = Quantity_Color {};
		colorTool->GetColor(shape, XCAFDoc_ColorGen, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorGen, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorSurf, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorCurv, color);

		// FIXME:
		actor->GetProperty()->SetColor(color.Red(), color.Green(), color.Blue());
		std::stringstream stringStream;
		stringStream << std::addressof(*actor.GetPointer());
		std::string actorKey = stringStream.str();

		actorsMap[actorKey] = actor;
	}

	return actorsMap;
}

vtkSmartPointer<vtkActor> Importing::STEPFileReader::createVTKActor(TopoDS_Shape shape) {
	IVtkOCC_Shape* vtkShapeAdapter = new IVtkOCC_Shape(shape);
	auto dataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
	dataSource->SetShape(vtkShapeAdapter);

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(dataSource->GetOutputPort());
	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	return actor;
}

std::string Importing::STEPFileReader::getUniqueObjectName(std::string prefix, const Importing::PartsMap& partsMap) {
	// find already existing path that match prefix
	std::vector<std::string> allNames;
	for (const auto& partsMapIt : partsMap) {
		// String object is the first element of parts map
		const std::string stringObj = partsMapIt.first;
		if (stringObj.find(prefix) != std::string::npos) {
			allNames.push_back(stringObj);
		}
	}

	int i = 0;
	std::string uniqueName;
	while (true) {
		std::stringstream stringStream;
		stringStream << prefix << std::setfill('0') << std::setw(3) << i;
		uniqueName = stringStream.str();
		auto res = std::find(std::begin(allNames), std::end(allNames), uniqueName);
		if (res == std::end(allNames)) {
			break;
		}
		i++;
	}
	return uniqueName;
}