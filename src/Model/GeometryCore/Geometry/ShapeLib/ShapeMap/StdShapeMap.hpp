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
 
#ifndef STDSHAPEMAP_HPP 
#define STDSHAPEMAP_HPP

#include "ShapeMap.hpp"
#include "IdRegistry.hpp"

#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>

#include <unordered_map>
#include <TopTools_ShapeMapHasher.hxx>
#include <exception>
#include <string>
#include <spdlog/spdlog.h>

class InvalidMapState : public std::exception {
private:
    std::string message;

public:
    explicit InvalidMapState(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class StdShapeMap : public ShapeMap {

    public:
    virtual ~StdShapeMap() = default;
 
    virtual bool containsId(const ShapeId& id) const override;
    virtual bool containsShape(const TopoDS_Shape& id) const override;
    
    virtual const TopoDS_Shape atId(const ShapeId& id) const override;
    virtual const ShapeId getId(const TopoDS_Shape& shape) const override;

    virtual const ShapeId registerTopLevelShape(
        const TopoDS_Shape& shape
    ) override;

    virtual const ShapeId registerSubShape(
        const TopoDS_Shape& shape, 
        const ShapeId& aParentId,
        int subShapeId
    ) override;

    virtual bool removeShape(const ShapeId& id) override;    

    virtual bool updateShape(
        const ShapeId& id,
        const TopoDS_Shape& shape
    ) override;
    
    private:
    bool insert(const std::pair<ShapeId, TopoDS_Shape>& aIdShapePair);

    IdRegistry _idRegistry;
    std::unordered_map<ShapeId, TopoDS_Shape, ShapeIdHasher> _idShapeMap;
    std::unordered_map<TopoDS_Shape, ShapeId, TopTools_ShapeMapHasher> _shapeIdMap;
};

#endif