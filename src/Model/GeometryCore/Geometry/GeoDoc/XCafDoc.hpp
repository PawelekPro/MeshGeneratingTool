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
#ifndef XCAFDOC_HPP
#define XCAFDOC_HPP

#include <string>

#include "OCafDoc.hpp"

class XCafDoc : public OcafDoc{

    public: 
    XCafDoc();
    virtual ~XCafDoc() = default;     

    virtual std::optional<TopoDS_Shape> getShape(const TDF_Label& aLabel) const override;
    virtual std::optional<TDF_Label> getLabel(const TopoDS_Shape& aShape) const override; 
    
    virtual std::vector<TDF_Label> importShapes(const OcafImporter& aImporter) override; 
    
    virtual void undo() override;
    virtual bool save(const std::string& filePath) const override;
    
    private:
    Handle(TDocStd_Document) _document;
    Handle(XCAFDoc_ShapeTool) _shapeTool;
    Handle(XCAFDoc_ColorTool) _colorTool;
};

#endif