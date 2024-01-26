#include <filesystem>

#include <vtkLogger.h>

#include <XCAFApp_Application.hxx>

#include "STEPFileReader.h"
#include <TDF_Attribute.hxx>
#include <TDataStd_Name.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

std::string Importing::STEPFileReader::load(const std::string& fileName) {
	if (!std::filesystem::exists(fileName)) {
		auto message = "File " + fileName + " can not be found.";
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::no_such_file_or_directory);
		throw std::filesystem::filesystem_error(message, errorCode);
	}
	auto baseName = std::filesystem::path { fileName }.stem().generic_string();
	vtkLogF(INFO, "Geometry file path: \"%s\"", fileName.c_str());

	auto doc = Handle(TDocStd_Document) {};
	auto app = XCAFApp_Application::GetApplication();
	app->NewDocument("MDTV-XCAF", doc);
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

	if (!cafReader.Transfer(doc)) {
		auto message = "Error while reading file:" + fileName;
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		throw std::filesystem::filesystem_error(message, errorCode);
	}

	vtkLogF(INFO, "STEPCAFControl_Reader transferred successfully.");

	auto& reader = cafReader.Reader();
	auto partsMap = Importing::PartsMap {};
	auto shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

	auto numberOfShapes = reader.NbShapes();
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

				auto uniqueName = getUniqueObjectName("Part", partsMap);
				std::stringstream stringStream;
				auto label = shapeTool->FindShape(solid);

				if (!label.IsNull()) {
					Handle(TDF_Attribute) attribute;
					if (label.FindAttribute(TDataStd_Name::GetID(), attribute)) {
						stringStream << Handle(TDataStd_Name)::DownCast(attribute)->Get();
					}
				}

				auto message = "STEP: Transferring Shape " + stringStream.str();
				vtkLogF(INFO, message.c_str());
				if (stringStream.str().size() == 0) {
					stringStream << "Part";
				}
				partsMap[uniqueName] = solid;
				std::cout << uniqueName.c_str() << std::endl;
			}
		}
	}

	return baseName;
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