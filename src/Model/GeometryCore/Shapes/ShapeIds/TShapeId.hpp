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

#ifndef TSHAPEID_HPP
#define TSHAPEID_HPP

#include "ShapeId.hpp"
#include <cstdint>

enum class ShapeType {
    Vertex,
    Edge,
    Face,
    Solid
};

class ShapeFactory;

class TShapeId : public ShapeId {

    friend class ShapeFactory;
    
    public:
    
    bool operator==(const TShapeId& aId){
        return (_id        == aId._id && 
                _shapeType == aId._shapeType);
    }
    
    bool operator>(const TShapeId& aId){
        if (_shapeType == aId._shapeType){
            return _id > aId._id; 
        } else { 
            return static_cast<int>(_shapeType) < static_cast<int>(aId._shapeType);
        }
    }
    
    private:
    
    explicit TShapeId(
        const ShapeType& aType,
        const uint64_t& aId) :
        _id(aId), 
        _shapeType(aType) {};
    
    uint64_t _id;
    ShapeType _shapeType;

};

#endif