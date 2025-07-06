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

#ifndef MOCKCUBEIMPORTER_HPP
#define MOCKCUBEIMPORTER_HPP 

#include "ShapeImporter.hpp"
#include "StubShapes.hpp"

#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_Label.hxx>

class MockCubeImporter : public ShapeImporter {
public:
    Handle(TDocStd_Document) import(
        const std::string& aFilePath,
        const ProgressIndicator& aProgressIndicator = IdleProgressIndicator()
    ) const override 
    {
        aProgressIndicator.progress("start", 0);
        Handle(TDocStd_Document) doc = initDocument();

        auto shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
        TopoDS_Shape cubeShape = StubShapes::cube();

        aProgressIndicator.progress("progress", 50);
        TDF_Label label = shapeTool->NewShape();
        shapeTool->SetShape(label, cubeShape);

        aProgressIndicator.progress("finish", 100);
        return doc;
    }
};

#endif