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
#include <TNaming_NamedShape.hxx>
#include <TDocStd_Modified.hxx>

OcafShapeCore::OcafShapeCore() {
    
    auto app = XCAFApp_Application::GetApplication();
    app->NewDocument("XmlXCAF", this->_document);
    app->InitDocument(_document);
    _document->SetUndoLimit(5);
    this->_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_document->Main());
    this->_colorTool = XCAFDoc_DocumentTool::ColorTool(this->_document->Main());
    XmlXCAFDrivers::DefineFormat(app);
    _shapeLabel = _shapeTool->Label();  
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

bool OcafShapeCore::registerNewShape(const TopoDS_Shape& aShape){
    TDF_Label newShapeLabel = _shapeTool->AddShape(aShape);
    _document->SetModified(newShapeLabel);

    return true;
}

bool OcafShapeCore::removeShape(const ShapeId& aShapeId) {
    if (!_shapeMap->containsId(aShapeId)) {
        std::cerr << "Error: ShapeId not found in map.\n";
        return false;
    }

    TopoDS_Shape shapeToRemove = _shapeMap->atId(aShapeId);
    
    TDF_Label labelToRemove = _shapeTool->FindShape(shapeToRemove);
    if (!labelToRemove.IsNull()){
        _shapeTool->RemoveShape(labelToRemove);
        return true;
    }
    
    return true;
}

bool OcafShapeCore::updateShape(
    const std::pair<ShapeId, TopoDS_Shape>& aShapeIdPair
) {
    const ShapeId& id = aShapeIdPair.first;
    const TopoDS_Shape& newShape = aShapeIdPair.second;
    if(!_shapeMap->containsId(id)){
        return false;
    }
    TopoDS_Shape shape = _shapeMap->atId(id);
    TDF_Label label = _shapeTool->FindShape(shape);
    if (!label.IsNull()){
        _shapeTool->SetShape(label, newShape);
        _document->SetModified(label);
    }
    return _shapeMap->updateShape(id, newShape);
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
    reviewDelta(_document->GetUndos().Last());
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
    reviewDelta(_document->GetRedos().Last());
    return true;
}

DeltaType attrDeltaType(Handle(TDF_AttributeDelta) aAttrDelta) {
    if (aAttrDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnRemoval))) {
        return DeltaType::Removal;
    }
    if (aAttrDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnAddition))) {
        return DeltaType::Addition;
    }
    if (aAttrDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnModification))) {
        return DeltaType::Modification;
    }
    if (aAttrDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnForget))) {
        return DeltaType::Forget;
    }
    if (aAttrDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnResume))) {
        return DeltaType::Resume;
    }
    throw std::runtime_error("Unknown delta type");
}

void OcafShapeCore::processShapeDelta(
    DeltaType aDeltaType,
    const TopoDS_Shape& aShape
){
    switch (aDeltaType) {
        case DeltaType::Addition:{
            TopTools_IndexedMapOfShape subShapeIds;
            TopExp::MapShapes(aShape, subShapeIds);
            ShapeId topLevelId = _shapeMap->registerTopLevelShape(aShape);
            for(int i = 1; i <= subShapeIds.Extent(); i++){
                const TopoDS_Shape subShape = subShapeIds(i);
                ShapeId subId = _shapeMap->registerSubShape(subShape, topLevelId);
            }
            _publisher.shapeAdded(topLevelId);
        }
            break;
        case DeltaType::Removal:{
            ShapeId id = _shapeMap->getId(aShape);
            bool shapeRemoved = _shapeMap->removeShape(id);
            _publisher.shapeRemoved(id);
        }
        break;
        case DeltaType::Modification:{
            ShapeId id = _shapeMap->getId(aShape);
            _publisher.shapeModified(id);
            break;
        }
        case DeltaType::Forget:
            break;
        case DeltaType::Resume:
            break;
        default:
            break;
    }
}

int OcafShapeCore::reviewDelta(Handle(TDF_Delta) aDelta) {
    TDF_AttributeDeltaList attrDeltaList = aDelta->AttributeDeltas();
    bool hasTnaming = false;
    for (auto attrDelta : attrDeltaList) {
        DeltaType deltaType = attrDeltaType(attrDelta);
        Handle(TDF_Attribute) attr = attrDelta->Attribute();
        TopoDS_Shape parentShape;
        if (deltaType == DeltaType::Removal){
            if (attr->IsKind(STANDARD_TYPE(XCAFDoc_ShapeMapTool))){
                Handle(XCAFDoc_ShapeMapTool) shapeMap = 
                    Handle(XCAFDoc_ShapeMapTool)::DownCast(attr);
                TopTools_IndexedMapOfShape removedSubShapes = shapeMap->GetMap();
                TopoDS_Shape subShape = removedSubShapes(1);
                ShapeId id = _shapeMap->getId(subShape);
                std::shared_ptr<const ShapeId> parentId = id.parentId();
                parentShape = _shapeMap->atId(*parentId);
            }
        } else {
            if (attr->IsKind(STANDARD_TYPE(TNaming_NamedShape))){
                Handle(TNaming_NamedShape) namedShape = 
                    Handle(TNaming_NamedShape)::DownCast(attr);
                parentShape = namedShape->Get();
            }
        }
        if (!parentShape.IsNull()){
            processShapeDelta(deltaType, parentShape);
        }
    }
    return attrDeltaList.Extent();
}

std::shared_ptr<const ShapeMap> OcafShapeCore::shapeMap() const {
    return _shapeMap;
};