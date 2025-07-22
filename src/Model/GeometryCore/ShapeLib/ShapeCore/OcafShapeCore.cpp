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

#include <TDF_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XmlXCAFDrivers.hxx>
#include <XmlDrivers.hxx>
#include <NCollection_IndexedMap.hxx>
#include <TNaming_NamedShape.hxx>
#include <TDocStd_Modified.hxx>
#include <Quantity_ColorRGBA.hxx>


#include "XmlShapeLibDrivers.hpp"
#include "CoreShapeMap.hpp"
#include "AttributeFactory.hpp"
#include "LabelKeyTool.hpp"
#include "OcafShapeRegistry.hpp"
#include "OcafShapeDocumentImport.hpp"

OcafShapeCore::OcafShapeCore()
{
    auto app = XCAFApp_Application::GetApplication();
    app->NewDocument("XmlXCAF", _document);
    app->InitDocument(_document);
    _document->SetUndoLimit(5);

    _shapeTool = XCAFDoc_DocumentTool::ShapeTool(_document->Main());
    _colorTool = XCAFDoc_DocumentTool::ColorTool(_document->Main());

    Handle(XmlMDF_ADriverTable) table = XmlDrivers::AttributeDrivers(app->MessageDriver());
    XmlShapeLibDrivers::AttributeDrivers(table, app->MessageDriver());   
    
    XmlDrivers::DefineFormat(app);
    XmlXCAFDrivers::DefineFormat(app);

    _shapeLabel = _shapeTool->Label();
    _shapeMap   = std::make_shared<CoreShapeMap>(_shapeTool);
    _attrFactory = std::move(std::make_unique<AttributeFactory>(_publisher));
    _shapeRegistry = std::make_shared<OcafShapeRegistry>(_document, _attrFactory);
}

bool OcafShapeCore::write(const std::string& aSavePath) const {
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

bool OcafShapeCore::importDocument(Handle(TDocStd_Document) aDoc) {
    OcafShapeDocumentImporter importer(_shapeRegistry);
    return importer.importDocument(aDoc);
}

bool OcafShapeCore::removeShape(const ShapeId& aShapeId) {
    auto label = LabelKeyTool::labelFromKey(
        _document->Main(), 
        ShapeIdFactory::getKey(aShapeId)
    );
    return _shapeTool->RemoveShape(label);
}

std::shared_ptr<Shape> OcafShapeCore::registerNewFreeShape(
    const TopoDS_Shape& aShape
) {
    return std::make_shared<OcafShape>(
        _shapeTool->NewShape(), OcafShapeTools(_shapeTool, _colorTool)
    );
}

bool OcafShapeCore::updateShape(
    const std::pair<ShapeId, TopoDS_Shape>& aShapeKeyPair
) {
    auto label = LabelKeyTool::labelFromKey(
        _document->Main(), ShapeIdFactory::getKey(aShapeKeyPair.first)
    );
    _shapeTool->SetShape(label, aShapeKeyPair.second);
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
    _document->CommitCommand();
    return true;
}

bool OcafShapeCore::abortCommand(){
    if (!_document->HasOpenCommand()){
        spdlog::warn("No command to abort.");
        return false;
    }
    _document->AbortCommand();
    return true;
}

bool OcafShapeCore::undo() {
    _document->Undo();
    return true;
}

bool OcafShapeCore::redo() {
    _document->Redo();
    return true;
}