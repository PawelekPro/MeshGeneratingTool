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

#include "OcafShapeDocumentImport.hpp"
#include "ShapeIdFactory.hpp"

#include "Shape.hpp"
#include <XCAFDoc_DocumentTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>

OcafShapeDocumentImporter::OcafShapeDocumentImporter(
    std::shared_ptr<ShapeRegistry> aDestRegistry
) : _destRegistry(aDestRegistry){}

bool OcafShapeDocumentImporter::importDocument(
    Handle(TDocStd_Document) aSourceDoc
) {
    _sourceDocument = aSourceDoc;

    _sourceShapeTool = XCAFDoc_DocumentTool::ShapeTool(
        aSourceDoc->Main()
    );
    _sourceColorTool = XCAFDoc_DocumentTool::ColorTool(
        aSourceDoc->Main()
    );
    TDF_LabelSequence freeShapes;
    _sourceShapeTool->GetFreeShapes(freeShapes);
    
    importLabelSequence(
        freeShapes,
        _destRegistry->baseLabel()
    );
    return true;
}

TDF_Label OcafShapeDocumentImporter::importAssembly(
    const TDF_Label& aSourceLabel,
    TDF_Label aDestParentLabel
) {
    TDF_Label assemblyLabel = importPart(
        aSourceLabel,
        aDestParentLabel
    );
    auto isNull = assemblyLabel.IsNull();
    TDF_LabelSequence components;
    _sourceShapeTool->GetComponents(aSourceLabel, components);
    importLabelSequence(
        components,
        assemblyLabel
    );
    return assemblyLabel;
}

TDF_Label OcafShapeDocumentImporter::importPart(
    const TDF_Label& aSourceLabel,
    TDF_Label aDestParentLabel
) {
    auto partImportData = extractShape(aSourceLabel);
    auto shape = _destRegistry->registerShape(
        partImportData,
        aDestParentLabel
    );
    auto trsf = shape->location().Transformation();
    auto key = ShapeIdFactory::getKey(shape->id());
    auto partLabel = LabelKeyTool::labelFromKey(
        _destRegistry->baseLabel(), key
    );
    return partLabel;
}

ShapeImportData OcafShapeDocumentImporter::extractShape(
    const TDF_Label& aLabel
) {
    auto shape = _sourceShapeTool->GetShape(aLabel);
    auto loc = _sourceShapeTool->GetLocation(aLabel);
    
    Handle(TDataStd_Name) nameAttr;
    aLabel.FindAttribute(TDataStd_Name::GetID(), nameAttr);
    TCollection_ExtendedString occName = nameAttr->Get();
    Standard_Integer utf8Len = occName.LengthOfCString();
    Standard_PCharacter utf8CString = new char[utf8Len];
    occName.ToUTF8CString(utf8CString);
    std::string name(utf8CString);
    ColorRGBA color;
    _sourceColorTool->GetColor(
        aLabel, XCAFDoc_ColorType::XCAFDoc_ColorGen, color
    );
    
    return ShapeImportData(shape, loc, name, color);
}

void OcafShapeDocumentImporter::importLabelSequence(
    const TDF_LabelSequence& aLabelSequence, 
    TDF_Label aDestParentLabel
) {
    for (Standard_Integer i = 1; i <= aLabelSequence.Length(); ++i) {
        auto label = aLabelSequence.Value(i);
        bool isAssembly = _sourceShapeTool->IsAssembly(label);
        if (isAssembly) {
            importAssembly(
                label, aDestParentLabel
            );
        } 
        else {
            importPart(
                label, aDestParentLabel
            );
        }
    }
}
