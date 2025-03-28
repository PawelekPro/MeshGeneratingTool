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

#include <XmlXCAFDrivers.hxx> 
#include <XCAFDoc_DocumentTool.hxx> 
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TDF_Label.hxx>

#include <TDocStd_Document.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Trsf.hxx>

#include "XcafDoc.hpp"

XCafDoc::XCafDoc() {
    auto app = XCAFApp_Application::GetApplication();
    app->NewDocument("XmlXCAF", this->_document);
    _document->SetUndoLimit(3);
    this->_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_document->Main());
    this->_colorTool = XCAFDoc_DocumentTool::ColorTool(this->_document->Main());
    XmlXCAFDrivers::DefineFormat(app);
    TDF_Label label;
    
}

bool XCafDoc::save(const std::string& filePath) const {
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


std::optional<TopoDS_Shape> getShape(const TDF_Label& aLabel){
   return std::nullopt; 
}

std::optional<TDF_Label> getLabel(const TopoDS_Shape& aShape){
    return std::nullopt;
}

std::vector<TDF_Label> importShapes(const OcafImporter& aImporter){
    return std::vector<TDF_Label>();
}

void XCafDoc::undo(){
    _document->Undo();
}