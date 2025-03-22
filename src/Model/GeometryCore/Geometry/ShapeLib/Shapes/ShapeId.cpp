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

#include "ShapeId.hpp"


ShapeId::ShapeId(
    uint64_t id,
    const ShapeType& type,
    std::shared_ptr<const ShapeId> parentId) : 
    _id(id),
    _shapeType(type),
    _parentId(parentId){}

bool ShapeId::operator==(const ShapeId& other) const {
    if (other.shapeType() != this->shapeType()) {
        return false;
    }

    auto thisParent = this->parentId();
    auto otherParent = other.parentId();

    if (thisParent && otherParent) { 
        if (thisParent->toInt() != otherParent->toInt()) {
            return false;
        }
    } else if (thisParent || otherParent) { 
        return false; 
    }

    return this->toInt() == other.toInt();
}

bool ShapeId::operator<(const ShapeId& other) const {
    if (this->shapeType() > other.shapeType()) {
        return true;
    } else if (this->shapeType() < other.shapeType()) {
        return false;
    }

    auto thisParent = this->parentId();
    auto otherParent = other.parentId();

    if (thisParent && otherParent) {  
        if (thisParent->toInt() > otherParent->toInt()) {
            return true;
        } else if (thisParent->toInt() < otherParent->toInt()) {
            return false;
        }
    } else if (!thisParent && otherParent) { 
        return false;
    } else if (thisParent && !otherParent) { 
        return true;
    }

    return this->toInt() > other.toInt();
}

const ShapeType ShapeId::shapeType() const {
    return _shapeType;
}

std::shared_ptr<const ShapeId> ShapeId::parentId() const {
    return _parentId;
};

std::string ShapeId::toString() const {
    std::string shapeId = std::to_string(static_cast<int>(_shapeType));
    std::string parentId;
    if(_parentId){
        parentId = std::to_string(_parentId->toInt());
    }
    std::string id = std::to_string(_id);
    return shapeId + "-" + parentId + "-" + id;
};

const uint64_t ShapeId::toInt() const { 
    return _id; 
};

