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

OcafShapeRegistry::OcafShapeRegistry(
    Handle(XCAFDoc_ShapeTool) aShapeTool,
    std::shared_ptr<AttributeFactory> aAttrFactory
) : _shapeTool(aShapeTool), 
    _attrFactory(aAttrFactory),
    _labelKeyTool(std::make_unique<LabelKeyTool>(aShapeTool)){}

    
TDF_Label OcafShapeRegistry::registerComponent(
    const ShapeData& aShapeData,
    TDF_Label& aLocalParent
) {
    return TDF_Label{};
};

TDF_Label OcafShapeRegistry::registerAssembly(
    const ShapeData& aShapeData,
    TDF_Label& aLocalParent
) {
   return TDF_Label{};
};


void OcafShapeRegistry::addShapeAttributeToLabel(
    TDF_Label& aLabel
) {
    // auto key = _labelKeyTool->keyFromLabel(aLabel);  
    // Handle(ShapePathAttr) attr = _attrFactory->shapePathAttr(
    //     key
    // );
    // aLabel.AddAttribute(attr);
}

void OcafShapeRegistry::addAssemblyAttributeToLabel(
    TDF_Label& aLabel
) {
    // auto key = _labelKeyTool->keyFromLabel(aLabel);  
    // Handle(AssemblyPathAttr) attr = _attrFactory->assemblyPathAttr(
    //     key
    // );
    // aLabel.AddAttribute(attr);
}