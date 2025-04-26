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

#ifndef IDREGISTRY_HPP
#define IDREGISTRY_HPP

#include "ShapeId.hpp"
#include <cstdint>
#include <set>
#include <TopoDS_Shape.hxx>

class IdRegistry{

    public:    
    IdRegistry() = default;
    ~IdRegistry() = default;
    
    ShapeId generateTopLevelId(const TopoDS_Shape& aShape){
        return ShapeId(_nextId, aShape.ShapeType());
    };

    ShapeId generateSubId(const TopoDS_Shape& aShape, const ShapeId& aParentId){
        std::shared_ptr<const ShapeId> parentId = std::make_shared<ShapeId>(aParentId);
        return ShapeId(_nextId, aShape.ShapeType(), parentId);
    };

    bool isBound(const ShapeId& aShapeId){
        if (_idSet.find(aShapeId._id) != _idSet.end()){
            return false;
        } else {
            return true;
        }
    }
    bool bindId(const ShapeId& aShapeId){
        if (_idSet.insert(aShapeId._id).second){
            _nextId++;
            return true;
        }
        return false;
    }

    private:
    uint64_t _nextId = 1;
    std::set<uint64_t> _idSet;
};

#endif