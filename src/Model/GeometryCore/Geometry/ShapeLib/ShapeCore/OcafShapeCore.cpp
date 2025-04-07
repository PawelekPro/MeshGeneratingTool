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
#include "OcafShapeMap.hpp"

#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XmlXCAFDrivers.hxx>

OcafShapeCore::OcafShapeCore() {
    
    auto app = XCAFApp_Application::GetApplication();
    app->NewDocument("XmlXCAF", this->_document);
    _document->SetUndoLimit(3);
    this->_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->_document->Main());
    this->_colorTool = XCAFDoc_DocumentTool::ColorTool(this->_document->Main());
    XmlXCAFDrivers::DefineFormat(app);
    
    auto mainLabel = _document->Main();
    TDF_Label shapeLabel = mainLabel.NewChild();
    TDataStd_Name::Set(shapeLabel, "Shapes");

    _shapeMap = std::make_shared<OcafShapeMap>();

}

const ShapeId OcafShapeCore::registerNewShape(const TopoDS_Shape& aShape){
    return ShapeId(0, ShapeType::Invalid);
}

bool OcafShapeCore::removeShape(const ShapeId& aShapeId){
    return true;
}

bool  OcafShapeCore::updateShape(
    const std::pair<ShapeId, TopoDS_Shape>& aShapeId
){
    return true; 
}

bool OcafShapeCore::openCommand(){
    return true;
}

bool OcafShapeCore::commitCommand(){
    return true;
}

bool OcafShapeCore::abortCommand(){
    return true;
}

bool OcafShapeCore::undo(){
    return true;
}

std::shared_ptr<const ShapeMap> OcafShapeCore::shapeMap() const {
    return _shapeMap;
};


boost::signals2::connection OcafShapeCore::connectShapeAdded(
    const std::function<void(const ShapeId&)>& slot
) {
    return _publisher.shapeAddedSignal().connect(slot); // ✅ Correct!
}

boost::signals2::connection OcafShapeCore::connectShapeRemoved(
    const std::function<void(const ShapeId&)>& slot
) {
    return _publisher.shapeRemovedSignal().connect(slot); // ✅ Consistent!
}

boost::signals2::connection OcafShapeCore::connectShapeModified(
    const std::function<void(const ShapeId&)>& slot
) {
    return _publisher.shapeModifiedSignal().connect(slot); // ✅ Good!
}