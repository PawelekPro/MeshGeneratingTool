#include "STEPImporter.hpp"
#include "OccProgressWrapper.hpp"
#include "ModelSubject.hpp"

DEFINE_STANDARD_HANDLE(OccProgressWrapper, Message_ProgressIndicator);

void GeometryCore::STEPImporter::import(const std::string& fileName, const ModelSubject& aModelSubject){

	Handle(OccProgressWrapper) progressWrapper = new OccProgressWrapper(aModelSubject, fileName);

	if (!std::filesystem::exists(fileName)) {
		auto message = "File " + fileName + " can not be found.";
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::no_such_file_or_directory);
		throw std::filesystem::filesystem_error(message, errorCode);
	}
	auto baseName = std::filesystem::path { fileName }.stem().generic_string();
	vtkLogF(INFO, "Geometry file path: \"%s\"", fileName.c_str());

	this->_dataFrame = Handle(TDocStd_Document) {};
	auto app = XCAFApp_Application::GetApplication();
	app->NewDocument("MDTV-XCAF", this->_dataFrame);
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
	if (!cafReader.Transfer(this->_dataFrame, progressWrapper->Start())) {
		auto message = "Error while reading file:" + fileName;
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		throw std::filesystem::filesystem_error(message, errorCode);
	}

	vtkLogF(INFO, "STEPCAFControl_Reader transferred successfully.");

	auto& reader = cafReader.Reader();
	this->_shapesMap = GeometryCore::PartsMap {};
	auto shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_dataFrame->Main());

	Standard_Integer numberOfShapes = reader.NbShapes();
	if (numberOfShapes == 0) {
		auto message = "No shapes found in given STEP file.";
		vtkLogF(ERROR, message);
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		throw std::filesystem::filesystem_error(message, errorCode);
	} else {
		for (auto i = 1; i <= numberOfShapes; i++) {
			auto shape = reader.Shape(i);
			std::string uniqueName = getUniqueObjectName("Shape", _shapesMap);
			this->_shapesMap[uniqueName] = shape;
		}
	}
	std::filesystem::path filePath(fileName);
	std::filesystem::path stepName = filePath.filename();
	auto message = "STEP file: " + stepName.string() + " loaded successfully.";
	vtkLogF(INFO, message.c_str());
};