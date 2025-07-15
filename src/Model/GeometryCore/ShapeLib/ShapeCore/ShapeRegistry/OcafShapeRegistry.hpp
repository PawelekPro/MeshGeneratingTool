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

#ifndef OCAFSHAPEREGISTRY_HPP
#define OCAFSHAPEREGISTRY_HPP

#include <TDocStd_Document.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <Standard_Handle.hxx>
#include <TDF_Label.hxx>

#include <memory>

#include "ShapeRegistry.hpp"
#include "AttributeFactory.hpp"
#include "LabelKeyTool.hpp"
#include "OcafShape.hpp"


class OcafShapeRegistry : public ShapeRegistry {

    public:
    OcafShapeRegistry(
        Handle(TDocStd_Document) aDocument,
        std::shared_ptr<AttributeFactory> aAttrFactory
    );

    virtual ~OcafShapeRegistry() = default;

    std::shared_ptr<Shape> registerShape(
        const ShapeImportData& aShapeData,
        TDF_Label aLocalParent = TDF_Label{}
    ) override;

    inline TDF_Label baseLabel() override { 
        return _shapeTool->BaseLabel();
    }

    private:
    void addPathAttribute(TDF_Label);
    
    void setNameAttribute(TDF_Label, const std::string&);
    void setColor(TDF_Label, const ColorRGBA&);

    Handle(TDocStd_Document) _document; 
    Handle(XCAFDoc_ColorTool) _colorTool; 
    Handle(XCAFDoc_ShapeTool) _shapeTool;
    std::shared_ptr<AttributeFactory> _attrFactory;
};

#endif