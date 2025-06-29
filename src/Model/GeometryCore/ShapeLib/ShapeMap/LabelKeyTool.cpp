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

#include "LabelKeyTool.hpp"
#include <TDF_Tool.hxx>

LabelKeyTool::LabelKeyTool(
    Handle(XCAFDoc_ShapeTool) aShapeTool
) : _shapeTool(aShapeTool){}

TDF_Label LabelKeyTool::labelFromKey(std::shared_ptr<ShapeKey> aKey){
    TColStd_ListOfInteger tagList;

    TDF_Tool::TagList(_shapeTool->Label(), tagList);
    
    for(auto treeNode : aKey->shapeTreePath()){
        tagList.Append(treeNode);
    }

    TDF_Label foundLabel;
    TDF_Tool::Label(_shapeTool->Label().Data(), tagList, foundLabel, false);
    return foundLabel;
}

std::shared_ptr<ShapeKey> LabelKeyTool::keyFromLabel(const TDF_Label& aLabel) {
    std::vector<int> tagPath;
    TDF_Label current = aLabel;

    while (!current.IsNull() && current != _shapeTool->Label()) {
        tagPath.push_back(current.Tag());
        current = current.Father();
    }
    std::reverse(tagPath.begin(), tagPath.end());

    return std::make_shared<ShapeKey>(tagPath);
}

std::shared_ptr<ShapeKey> LabelKeyTool::keyFromAttr(Handle(LabelPathAttr) aAttr) {
    return std::make_shared<ShapeKey>(aAttr->shapeTreePath());
}
