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

#include "XmlShapeLibDrivers.hpp"
#include "CoreShapeMap.hpp"
#include "AttributeFactory.hpp"
#include "LabelKeyTool.hpp"

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
    _labelKeyTool = std::move(std::make_unique<LabelKeyTool>(_shapeTool));
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

std::shared_ptr<ShapeKey> OcafShapeCore::registerNewFreeShape(const TopoDS_Shape& aShape) {
    TDF_Label mainLabel = _shapeTool->AddShape(aShape);

    Standard_Integer mainLabelTag = mainLabel.Tag();
    Handle(ShapeKeyAttr) mainAttr = _attrFactory->shapeKeyAttr(
        std::make_shared<ShapeKey>(mainLabelTag, 0)
    );
    
    mainLabel.AddAttribute(mainAttr);
    for (TopExp_Explorer exp(aShape, TopAbs_SHAPE); exp.More(); exp.Next()) {
        const TopoDS_Shape& subShape = exp.Current();
        if (subShape.IsEqual(aShape))
            continue;
        TDF_Label subLabel = _shapeTool->AddShape(subShape);
        Standard_Integer subLabelTag = subLabel.Tag();
        
        Handle(ShapeKeyAttr) subAttr = _attrFactory->shapeKeyAttr(
            std::make_shared<ShapeKey>(subLabelTag, mainLabelTag)
        );
        subLabel.AddAttribute(subAttr);
    }
    return _labelKeyTool->keyFromLabel(mainLabel);
}

bool OcafShapeCore::removeShape(std::shared_ptr<ShapeKey> aShapeKey) {
    auto label = _labelKeyTool->labelFromKey(aShapeKey);
    return _shapeTool->RemoveShape(label);
}

bool OcafShapeCore::updateShape(
    const std::pair<std::shared_ptr<ShapeKey>, TopoDS_Shape>& aShapeKeyPair
) {
    auto label = _labelKeyTool->labelFromKey(aShapeKeyPair.first);
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