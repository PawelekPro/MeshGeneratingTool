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

#ifndef LABELSHAPEMAP_HPP
#define LABELSHAPEMAP_HPP

#include <XCAFDoc_ShapeTool.hxx>
#include "ShapeMap.hpp"

class LabelShapeMap : public ShapeMap {

    public:
    LabelShapeMap(Handle(XCAFDoc_ShapeTool) aShapeTool);
    ~LabelShapeMap() = default;
 
    bool containsId(const ShapeId& id) const override;
    bool containsShape(const TopoDS_Shape& id) const override;
    
    const TopoDS_Shape atId(const ShapeId& id) const override;
    const ShapeId getId(const TopoDS_Shape& shape) const override;

    protected:    

    const ShapeId registerTopLevelShape(
        const TopoDS_Shape& shape
    ) override;

    const ShapeId registerSubShape(
        const TopoDS_Shape& shape, 
        const ShapeId& parentId
    ) override;
    
    bool removeShape(const ShapeId& id) override;

    bool updateShape(
        const ShapeId& id,
        const TopoDS_Shape& shape
    ) override;
    
    private:
    static TDF_Label idLabel(const ShapeId& id);
    ShapeId shapeId(const TDF_Label& label) const;
    
    Handle(XCAFDoc_ShapeTool) _shapeTool;

};
#endif