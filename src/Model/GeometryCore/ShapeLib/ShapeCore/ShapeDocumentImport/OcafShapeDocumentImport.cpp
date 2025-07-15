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
#include <XCAFDoc_DocumentTool.hxx>

bool ShapeDocumentImport::importDocument(
    std::shared_ptr<ShapeRegistry> aDestRegistry,
    Handle(TDocStd_Document) aSourceDoc
) {
    Handle(XCAFDoc_ShapeTool) sourceShapeTool = XCAFDoc_DocumentTool::ShapeTool(
        aSourceDoc->Main()
    );

    TDF_LabelSequence freeShapes;
    sourceShapeTool->GetFreeShapes(freeShapes);
    
    for (Standard_Integer i = 1; i <= freeShapes.Length(); ++i) {
        auto label = freeShapes.Value(i);
        bool isAssembly = sourceShapeTool->IsAssembly(label);
        if (isAssembly) {
            ShapeDocumentImport::importComponent(
                sourceShapeTool,
                label,
                aDestRegistry,
                aDestRegistry->baseLabel()
            );
        } 
        else {
            ShapeDocumentImport::importPart(
                sourceShapeTool,
                label,
                aDestRegistry,
                aDestRegistry->baseLabel()
            );
        }
    }
    return true;
}

bool ShapeDocumentImport::importComponent(
    Handle(XCAFDoc_ShapeTool) aSourceTool,
    const TDF_Label& aSourceLabel,
    std::shared_ptr<ShapeRegistry> aDestRegistry,
    TDF_Label aDestParentLabel
) {
    return true;
}

bool ShapeDocumentImport::importPart(
    Handle(XCAFDoc_ShapeTool) aSourceTool,
    const TDF_Label& aSourceLabel,
    std::shared_ptr<ShapeRegistry> aDestRegistry,
    TDF_Label aDestParentLabel
) {
    return true;
}

ShapeImportData ShapeDocumentImport::extractShape(
    Handle(XCAFDoc_ShapeTool) aSourceTool,
    const TDF_Label& aLabel
) {
    return ShapeImportData(TopoDS_Shape(), TopLoc_Location(), "shape", ColorRGBA());
}