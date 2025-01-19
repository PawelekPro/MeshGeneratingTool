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


OcafDoc::OcafDoc(){
    auto app = XCAFApp_Application::GetApplication();
    app->NewDocument("MDTV-XCAF", this->_document);
    this->_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_document->Main());
    this->_colorTool = XCAFDoc_DocumentTool::ColorTool(this->_document->Main());
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

void OcafDoc::importSTEP(const std::string& aFilePath){

    STEPCAFControl_Reader cafReader;

    cafReader.SetColorMode(true);
    cafReader.SetLayerMode(true);
    cafReader.SetNameMode(true);
    IFSelect_ReturnStatus result = cafReader.ReadFile(aFilePath.c_str());

    if (result != IFSelect_RetDone) {
        throw std::runtime_error("Failed to read STEP file.");
    }

    if (!cafReader.Transfer(this->_document)) {
        throw std::runtime_error("Failed to transfer STEP data to OCAF document.");
    }
}

void OcafDoc::undo(){
    _document->Undo();
}