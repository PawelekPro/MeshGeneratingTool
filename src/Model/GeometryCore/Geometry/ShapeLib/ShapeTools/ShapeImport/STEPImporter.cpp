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
#include "STEPImporter.hpp"
#include "OccProgressIndicator.hpp"
#include <stdexcept>

#include <STEPCAFControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <Quantity_ColorRGBA.hxx>

std::vector<std::pair<TopoDS_Shape, ShapeAttr>> STEPImporter::import(
    std::istream& aFileStream, 
    const ProgressIndicator& aProgressIndicator
) const {
    STEPCAFControl_Reader reader;
    reader.SetColorMode(true);
    reader.SetNameMode(true);

    IFSelect_ReturnStatus result = reader.ReadStream("STEPFileStream", aFileStream);
    if (result != IFSelect_RetDone) {
        throw std::runtime_error("Failed to read STEP file.");
    }

    auto app = XCAFApp_Application::GetApplication();
    Handle(TDocStd_Document) document;
    app->NewDocument("XmlXCAF", document);

    OccProgressIndicator progressIndicator(aProgressIndicator);
    reader.Transfer(document, progressIndicator.Start());

    Handle(XCAFDoc_ColorTool) colorTool = 
        XCAFDoc_DocumentTool::ColorTool(document->Main());

    auto shapeLabels = reader.GetShapeLabelMap();
    XCAFDoc_DataMapIteratorOfDataMapOfShapeLabel mapIter(shapeLabels);

    std::vector<std::pair<TopoDS_Shape, ShapeAttr>> shapes;
    for (auto it = mapIter; it.More(); it.Next()) {
        const TopoDS_Shape shape = it.Key();
        const TDF_Label label = it.Value();

        Handle(TDataStd_Name) nameAttribute;
        std::string name = "name";
        if (label.FindAttribute(TDataStd_Name::GetID(), nameAttribute)) {
            auto shapeName = nameAttribute->Get();
            Standard_Integer requiredLength = shapeName.Length() + 1;
            Standard_PCharacter utf8CString = new char[requiredLength];
            shapeName.ToUTF8CString(utf8CString);
            name = utf8CString;
            delete[] utf8CString;
        }

        Quantity_ColorRGBA colorRGBA;
        colorTool->GetColor(label, colorRGBA);
        Quantity_Color color = colorRGBA.GetRGB();
        ShapeColor shapeColor;
        shapeColor.r = color.Red(); 
        shapeColor.g = color.Green(); 
        shapeColor.b = color.Blue(); 
        ShapeAttr shapeAttrs;
        shapeAttrs.name = name;
        shapeAttrs.color = shapeColor;
        std::pair<TopoDS_Shape, ShapeAttr> pair(shape, shapeAttrs);
        shapes.push_back(pair);
    }

    return shapes;
}
