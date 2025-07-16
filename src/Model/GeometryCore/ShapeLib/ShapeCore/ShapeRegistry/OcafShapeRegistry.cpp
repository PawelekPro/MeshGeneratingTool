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

#include "OcafShapeRegistry.hpp"
#include <TDataStd_Name.hxx>
#include <Quantity_ColorRGBA.hxx>

OcafShapeRegistry::OcafShapeRegistry(
    Handle(TDocStd_Document) aDocument,
    std::shared_ptr<AttributeFactory> aAttrFactory
) : 
_document(aDocument),
_colorTool(XCAFDoc_DocumentTool::ColorTool(aDocument->Main())),
_shapeTool(XCAFDoc_DocumentTool::ShapeTool(aDocument->Main())),
_attrFactory(aAttrFactory){}
    
std::shared_ptr<Shape> OcafShapeRegistry::registerShape(
    const ShapeImportData& aShapeData,
    TDF_Label aLocalParent
) {
    TopoDS_Shape locatedShape = aShapeData.shape.Located(aShapeData.location);


    if(aLocalParent.IsNull()){
        TDF_Label foundLabel;
        if (_shapeTool->FindShape(locatedShape, foundLabel, true)){
            throw Exceptions::ShapeRegistry::ShapeAlreadyRegistered(
                "Trying to register the same shape twice in identical location."
            );
        }
    }

    TDF_Label label;
    if (aLocalParent.IsNull()){
        label = _shapeTool->AddShape(
            locatedShape
        );
    } else {
        label = _shapeTool->AddComponent(
            aLocalParent,
            locatedShape
        );
    }

    addPathAttribute(label);
    setNameAttribute(label, aShapeData.name);
    setColor(label, aShapeData.color);

    return std::make_shared<OcafShape>(
        label, 
        OcafShapeTools(_shapeTool, _colorTool) 
    ); 
}

void OcafShapeRegistry::addPathAttribute(
    TDF_Label aLabel
) {
    auto key = LabelKeyTool::keyFromLabel(aLabel);
    Handle(ShapePathAttr) attr = _attrFactory->shapePathAttr(
        key
    );
    aLabel.AddAttribute(attr);
}

void OcafShapeRegistry::setNameAttribute(
    TDF_Label aLabel,
    const std::string& aName
){
    TDataStd_Name::Set(
        aLabel, 
        TCollection_ExtendedString(
            aName.c_str()
        )
    );
}

void OcafShapeRegistry::setColor(
    TDF_Label aLabel,
    const ColorRGBA& aColor
) {
    auto shape = _shapeTool->GetShape(aLabel);
    _colorTool->SetColor(shape, aColor, XCAFDoc_ColorType::XCAFDoc_ColorGen);
}