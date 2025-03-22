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
 
#ifndef OCAFSHAPEMAP_HPP 
#define OCAFSHAPEMAP_HPP

#include "ShapeMap.hpp"
#include "OcafDoc.hpp"

#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>

#include <map>

class OcafDoc;

class OcafShapeMap : public ShapeMap {
    
    public:
    OcafShapeMap(const OcafDoc& aOcafDoc);
    virtual ~OcafShapeMap() = default;
 
    virtual bool containsId(const ShapeId& id) const override;
    virtual bool containsShape(const TopoDS_Shape& id) const override;
    
    virtual const TopoDS_Shape atId(const ShapeId& id) const override;
    virtual const ShapeId getId(const TopoDS_Shape& shape) const override;

    protected:
    
    virtual const ShapeId registerShape(const TopoDS_Shape& shape) override;
    virtual bool removeShape(const ShapeId& id) override;    
    virtual bool updateShape(
        const ShapeId& id,
        const TopoDS_Shape& shape
    ) override;
    
    private:

    const OcafDoc& _ocafDoc;

    std::map<const ShapeId, TDF_Label> _idLabelMap;
    std::map<TDF_Label, const ShapeId> _labelIdMap;

    std::map<TopoDS_Shape, TDF_Label> _shapeLabelMap; 
    std::map<TDF_Label, TopoDS_Shape> _labelShapeMap; 
};

#endif