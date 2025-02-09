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
#include "STEPOcafImporter.hpp"
#include "STLOcafImporter.hpp"


#include "XCAFApp_Application.hxx"
#include "XCAFDoc_DocumentTool.hxx"

#include <XmlXCAFDrivers.hxx>
#include <TDocStd_Application.hxx>
#include <Standard_Failure.hxx>
#include <Storage_Error.hxx>
#include "XmlXCAFDrivers.hxx"


OcafDoc::OcafDoc(const IEventSubject& aModelSubject) : _modelSubject(aModelSubject){
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
        std::cerr << "ShapeTool not initialized" << std::endl;
        return shapes;
    }
    TDF_LabelSequence freeShapes;
    _shapeTool->GetFreeShapes(freeShapes);
    shapes.resize(freeShapes.Length());
    std::transform(
        freeShapes.begin(), freeShapes.end(), shapes.begin(),
        [this](const TDF_Label& label) { return _shapeTool->GetShape(label); }
    );

    return shapes;
}

bool OcafDoc::import(OcafImporter& aImporter, const std::string& aFilePath){
    bool result = false;
    try {
        _document->NewCommand();
        result = aImporter.importToDocument(aFilePath,  _document);
        _document->CommitCommand();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Could not import file, error: " << e.what() << std::endl;
    } 
    catch (const std::exception& e) {
        std::cerr << "Could not import file, error: " << e.what() << std::endl;
    } 
    catch (...) { 
        std::cerr << "Could not import file, unknown exception" << std::endl;
    }
    return result;
}


bool OcafDoc::importSTL(const std::string& aFilePath) {
    STLOcafImporter importer(_modelSubject);
    return import(importer, aFilePath);
}

bool OcafDoc::importSTEP(const std::string& aFilePath) {
    STEPOcafImporter importer(_modelSubject);
    return import(importer, aFilePath);
}


void OcafDoc::undo(){
    _document->Undo();
}