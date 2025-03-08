/*
 * Copyright (C) 2024 Krystian Fudali
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
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

#include "OcafDoc.hpp"
#include "XCAFApp_Application.hxx"
#include "XCAFDoc_DocumentTool.hxx"
#include "STEPCAFControl_Reader.hxx"
#include <XmlXCAFDrivers.hxx>
#include <TDocStd_Application.hxx>
#include <Standard_Failure.hxx>
#include <Storage_Error.hxx>
#include "XmlXCAFDrivers.hxx"
#include <locale>
#include <codecvt>
OcafDoc::OcafDoc(){
    auto app = XCAFApp_Application::GetApplication();
    app->NewDocument("XmlXCAF", this->_document);
    _document->SetUndoLimit(3);
    this->_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_document->Main());
    this->_colorTool = XCAFDoc_DocumentTool::ColorTool(this->_document->Main());
    XmlXCAFDrivers::DefineFormat(app);
}

bool OcafDoc::saveAsXml(const std::string& filePath) const {
    try {
        if (_document.IsNull()) {
            throw std::runtime_error("Document is null and cannot be saved.");
        }

        std::string fullPath = filePath;
        if (fullPath.find(".xml") == std::string::npos) {
            fullPath += ".xml";
        }

        Handle(TDocStd_Application) app = XCAFApp_Application::GetApplication();
        if (app.IsNull()) {
            throw std::runtime_error("XCAF application is not initialized.");
        }
        std::u16string utf16Path;
        for (char c : fullPath) {
            utf16Path.push_back(static_cast<char16_t>(c));
        }
        Standard_ExtString occFilePath = utf16Path.c_str();
        
        app->SaveAs(_document, occFilePath);
        return true;
    } catch (const Standard_Failure& e) {
        std::cerr << "OpenCASCADE Error: " << e.GetMessageString() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return false;
}

std::vector<TopoDS_Shape> OcafDoc::getAllShapes() const {
    std::vector<TopoDS_Shape> shapes;

    if (!_shapeTool) {
        return shapes;
    }

    TDF_LabelSequence freeShapes;
    _shapeTool->GetFreeShapes(freeShapes); // Retrieve free shapes

    for (Standard_Integer i = 1; i <= freeShapes.Length(); ++i) {
        TDF_Label shapeLabel = freeShapes.Value(i);

        if (_shapeTool->IsShape(shapeLabel)) {
            TopoDS_Shape shape = _shapeTool->GetShape(shapeLabel);
            shapes.push_back(shape);
        }
    }
    return shapes;
}

void OcafDoc::importSTEP(const std::string& aFilePath) {
    STEPCAFControl_Reader cafReader;

    cafReader.SetColorMode(true);
    cafReader.SetLayerMode(true);
    cafReader.SetNameMode(true);
    IFSelect_ReturnStatus result = cafReader.ReadFile(aFilePath.c_str());

    if (result != IFSelect_RetDone) {
        throw std::runtime_error("Failed to read STEP file.");
    }

    _document->NewCommand();
    if (!cafReader.Transfer(this->_document)) {
        throw std::runtime_error("Failed to transfer STEP data to OCAF document.");
    }
    _document->CommitCommand();
}

void OcafDoc::undo(){
    _document->Undo();
}