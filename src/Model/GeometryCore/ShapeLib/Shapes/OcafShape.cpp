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

#include "OcafShape.hpp"
#include "ShapeIdFactory.hpp"
#include "LabelKeyTool.hpp"
#include <Quantity_ColorRGBA.hxx>
#include <Quantity_Color.hxx>
#include <TDataStd_Name.hxx>
#include <Standard_TypeDef.hxx>

OcafShape::OcafShape(
    TDF_Label aShapeLabel,
    OcafShapeTools aTools
) : _label(aShapeLabel), 
    _shapeTool(aTools.shapeTool),
    _colorTool(aTools.colorTool) {
        auto key = LabelKeyTool::keyFromLabel(aShapeLabel);
        _id = ShapeIdFactory::create(key);
    }

bool OcafShape::isAssembly() const {
    return _shapeTool->IsAssembly(_label);    
}; 

TopoDS_Shape OcafShape::shape() const {
    return _shapeTool->GetShape(_label);    
}; 

TopLoc_Location OcafShape::location() const {
    return _shapeTool->GetLocation(_label);    
}; 

std::string OcafShape::name() const {
    Handle(TDataStd_Name) nameAttr;
    _label.FindAttribute(TDataStd_Name::GetID(), nameAttr);
    TCollection_ExtendedString occName = nameAttr->Get();
    Standard_Integer utf8Len = occName.LengthOfCString();
    Standard_PCharacter utf8CString = new char[utf8Len];
    occName.ToUTF8CString(utf8CString);
    std::string name(utf8CString);
    return name;
}

ColorRGBA OcafShape::color() const {
    Quantity_ColorRGBA colorRGBA;
    _colorTool->GetColor(_label, XCAFDoc_ColorType::XCAFDoc_ColorGen, colorRGBA);
    return colorRGBA;
};