/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 *
 * Created by Paweł Gilewicz on 01/02/2024.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "STEPFileReader.h"

void Importing::STEPFileReader::load(const std::string& fileName) {
	_progressBar->initialize();
	_progressBar->setProgressMessage("Loading parts...");

	if (!std::filesystem::exists(fileName)) {
		auto message = "File " + fileName + " can not be found.";
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::no_such_file_or_directory);
		_progressBar->finish();
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
		_progressBar->finish();
		throw std::filesystem::filesystem_error(message, errorCode);
	}

	if (!cafReader.Transfer(this->_dataFrame)) {
		auto message = "Error while reading file:" + fileName;
		vtkLogF(ERROR, message.c_str());
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		_progressBar->finish();
		throw std::filesystem::filesystem_error(message, errorCode);
	}

	vtkLogF(INFO, "STEPCAFControl_Reader transferred successfully.");

	auto& reader = cafReader.Reader();
	this->_partsMap = Importing::PartsMap {};
	auto shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_dataFrame->Main());

	Standard_Integer numberOfShapes = reader.NbShapes();
	if (numberOfShapes == 0) {
		auto message = "No shapes found in given STEP file.";
		vtkLogF(ERROR, message);
		auto errorCode = std::make_error_code(std::errc::device_or_resource_busy);
		_progressBar->finish();
		throw std::filesystem::filesystem_error(message, errorCode);
	} else {
		for (auto i = 1; i <= numberOfShapes; i++) {
			int progress = static_cast<int>(100.0 * i / numberOfShapes);
			_progressBar->setValue(progress);

			auto _shape = reader.Shape(i);

			// Load each solid as an own object
			auto explorer = TopExp_Explorer {};
			for (explorer.Init(_shape, TopAbs_SOLID); explorer.More(); explorer.Next()) {

				// Get the shape
				auto& solid = TopoDS::Solid(explorer.Current());

				auto uniqueName = getUniqueObjectName("SolidPart");

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
				this->_partsMap[uniqueName] = solid;
				// std::cout << uniqueName.c_str() << std::endl;
			}

			// Load each non-solid part
			for (explorer.Init(_shape, TopAbs_SHELL, TopAbs_SOLID); explorer.More(); explorer.Next()) {
				// Get the shape
				auto _shell = TopoDS::Shell(explorer.Current());

				auto uniqueName = getUniqueObjectName("ShellPart");

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
				this->_partsMap[uniqueName] = _shell;
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
				std::string uniqueName = getUniqueObjectName("CompoundPart");
				this->_partsMap[uniqueName] = compound;
			}
		}
	}

	std::filesystem::path filePath(fileName);
	std::filesystem::path stepName = filePath.filename();
	auto message = "STEP file: " + stepName.string() + " loaded successfully.";
	_progressBar->finish();
	vtkLogF(INFO, message.c_str());
}

Importing::ActorsMap Importing::STEPFileReader::getVTKActorsMap() {
	Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(this->_dataFrame->Main());
	Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_dataFrame->Main());

	Importing::ActorsMap actorsMap {};

	for (const auto& it : this->_partsMap) {
		const auto& shape = it.second;

		vtkSmartPointer<vtkActor> actor = createVTKActor(shape);

		auto color = Quantity_Color {};
		colorTool->GetColor(shape, XCAFDoc_ColorGen, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorGen, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorSurf, color);
		colorTool->GetInstanceColor(shape, XCAFDoc_ColorCurv, color);

		actor->GetProperty()->SetColor(color.Red(), color.Green(), color.Blue());
		std::stringstream stringStream;
		stringStream << std::addressof(*actor.GetPointer());
		std::string actorKey = stringStream.str();

		actorsMap[actorKey] = actor;
	}

	return actorsMap;
}
