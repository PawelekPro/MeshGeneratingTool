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

#include "OcafShapeCore.hpp"
#include "StdShapeMap.hpp"

#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XmlXCAFDrivers.hxx>
#include <NCollection_IndexedMap.hxx>

OcafShapeCore::OcafShapeCore() {
    
    auto app = XCAFApp_Application::GetApplication();
    app->NewDocument("XmlXCAF", this->_document);
    _document->SetUndoLimit(3);
    this->_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_document->Main());
    this->_colorTool = XCAFDoc_DocumentTool::ColorTool(this->_document->Main());
    XmlXCAFDrivers::DefineFormat(app);
    
    _shapeMap = std::make_shared<StdShapeMap>();

}

bool OcafShapeCore::write(const std::string& aSavePath){
    if (_document.IsNull()) {
        std::cerr << "Error: Document is null. Cannot save.\n";
        return false;
    }

    TCollection_ExtendedString xmlPath(aSavePath.c_str());
    XmlXCAFDrivers::DefineFormat(XCAFApp_Application::GetApplication());

    if (!XCAFApp_Application::GetApplication()->SaveAs(_document, xmlPath)) {
        std::cerr << "Error: Failed to save document to " << aSavePath << std::endl;
        return false;
    }

    return true;
}
const ShapeId OcafShapeCore::registerNewShape(const TopoDS_Shape& aShape){
    TDF_Label newShapeLabel = _shapeTool->AddShape(aShape);

    TopTools_IndexedMapOfShape subShapeIds;
    TopExp::MapShapes(aShape, subShapeIds);

    ShapeId topLevelId = _shapeMap->registerTopLevelShape(aShape);
    for(int i = 0; i < subShapeIds.Extent(); i++){
        const TopoDS_Shape subShape = subShapeIds(i);
        ShapeId subId = _shapeMap->registerSubShape(subShape, topLevelId, i);
    }
    return topLevelId;
}

bool OcafShapeCore::removeShape(const ShapeId& aShapeId) {
    if (!_shapeMap->containsId(aShapeId)) {
        std::cerr << "Error: ShapeId not found in map.\n";
        return false;
    }

    TopoDS_Shape shapeToRemove = _shapeMap->atId(aShapeId);
    TDF_Label labelToRemove = _shapeTool->FindShape(shapeToRemove);

    if (!_shapeTool->RemoveShape(labelToRemove)) {
        std::cerr << "Warning: Could not remove shape from document.\n";
    }

    if (!_shapeMap->removeShape(aShapeId)) {
        std::cerr << "Error: Failed to remove shape from map.\n";
        return false;
    }

    return true;
}


bool OcafShapeCore::updateShape(
    const std::pair<ShapeId, TopoDS_Shape>& aShapeIdPair
) {
    const ShapeId& id = aShapeIdPair.first;
    const TopoDS_Shape& newShape = aShapeIdPair.second;

    if (!_shapeMap->containsId(id)) {
        std::cerr << "Error: ShapeId not found in map.\n";
        return false;
    }

    TopoDS_Shape oldShape = _shapeMap->atId(id);
    TDF_Label labelToRemove = _shapeTool->FindShape(oldShape);
    if (!_shapeTool->RemoveShape(labelToRemove)) {
        std::cerr << "Warning: Could not remove old shape from document.\n";
    }

    TDF_Label newLabel = _shapeTool->AddShape(newShape);

    if (!_shapeMap->updateShape(id, newShape)) {
        std::cerr << "Error: Failed to update shape in shape map.\n";
        return false;
    }

    return true;
}


bool OcafShapeCore::openCommand(){
    if (_document->HasOpenCommand()){
        spdlog::warn("A command is already open on the document.");
        return false;
    }
    _document->OpenCommand();
    return _document->HasOpenCommand();
}

bool OcafShapeCore::commitCommand(){
    if (!_document->HasOpenCommand()){
        spdlog::warn("No command has been opened, cannot commit.");
        return false;
    }
    return _document->CommitCommand();
}

bool OcafShapeCore::abortCommand(){
    if (!_document->HasOpenCommand()){
        spdlog::warn("No command to abort.");
        return false;
    }
    _document->AbortCommand();
    return true;
}

bool OcafShapeCore::undo(){
    return _document->Undo();
}

std::shared_ptr<const ShapeMap> OcafShapeCore::shapeMap() const {
    return _shapeMap;
};
