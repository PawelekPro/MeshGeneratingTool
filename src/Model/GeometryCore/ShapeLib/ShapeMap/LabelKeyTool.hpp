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

#ifndef LABELKEYTOOL_HPP
#define LABELKEYTOOL_HPP

#include <TDF_Label.hxx>
#include <Standard_Handle.hxx>
#include <XCAFDoc_ShapeTool.hxx>

#include "ShapeKey.hpp"
#include "LabelPathAttr.hpp"

class LabelKeyTool {

    public: 
    LabelKeyTool(Handle(XCAFDoc_ShapeTool) aShapeTool);
    ~LabelKeyTool() = default;
    
    TDF_Label labelFromKey(std::shared_ptr<ShapeKey> aKey);

    std::shared_ptr<ShapeKey> keyFromLabel(const TDF_Label& aLabel);
    std::shared_ptr<ShapeKey> keyFromAttr(Handle(LabelPathAttr) aAttribute);

    private:
    Handle(XCAFDoc_ShapeTool) _shapeTool;
};

#endif