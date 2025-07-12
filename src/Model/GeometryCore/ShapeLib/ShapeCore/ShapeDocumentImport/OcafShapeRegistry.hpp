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

#include <XCAFDoc_ShapeTool.hxx>
#include <TDocStd_Document.hxx>
#include <Standard_Handle.hxx>
#include <TDF_Label.hxx>

#include <memory>

#include "ShapeRegistry.hpp"
#include "AttributeFactory.hpp"
#include "LabelKeyTool.hpp"

class OcafShapeRegistry : public ShapeRegistry {

    public:
    OcafShapeRegistry(
        Handle(XCAFDoc_ShapeTool) aShapeTool,
        std::shared_ptr<AttributeFactory> attrFactory
    );

    virtual ~OcafShapeRegistry() = default;

    TDF_Label registerComponent(
        const ShapeData& aShapeData,
        TDF_Label& aLocalParent
    ) override;

    TDF_Label registerAssembly(
        const ShapeData& aShapeData,
        TDF_Label& aLocalParent
    ) override;

    inline TDF_Label baseLabel() override { 
        return _shapeTool->BaseLabel();
    }

    private:
    void addShapeAttributeToLabel(TDF_Label&);
    void addAssemblyAttributeToLabel(TDF_Label&);

    Handle(XCAFDoc_ShapeTool) _shapeTool;
    std::unique_ptr<LabelKeyTool> _labelKeyTool;
    std::shared_ptr<AttributeFactory> _attrFactory;
};

#endif