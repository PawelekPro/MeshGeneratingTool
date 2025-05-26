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
    std::unique_ptr<ShapeKey> aKey,
    std::shared_ptr<const ShapeId> aParentId
) : _key(std::move(aKey)),
    _parentId(std::move(aParentId)){}

ShapeId::ShapeId(const ShapeId& aOther) noexcept
    : _key       ( aOther._key->clone() )
    , _parentId  ( aOther._parentId ) {}

ShapeId::ShapeId(ShapeId&& aOther) noexcept
    : _key       ( std::move(aOther._key) )
    , _parentId  ( std::move(aOther._parentId) ) {}

ShapeId& ShapeId::operator=(const ShapeId& aOther) noexcept{
    if (this != &aOther) {
      _key      = aOther._key->clone();
      _parentId = aOther._parentId;
    }
    return *this;
}   

ShapeId& ShapeId::operator=(ShapeId&& aOther) noexcept {
    if (this != &aOther) {
      _key      = aOther._key->clone();
      _parentId = aOther._parentId;
    }
    return *this;
}

bool ShapeId::operator==(const ShapeId& other) const {
    return _key->equals(*other._key);
}

bool ShapeId::operator<(const ShapeId& other) const {
    return _key->less(*other._key);
}

std::shared_ptr<const ShapeId> ShapeId::parentId() const {
    return _parentId;
}

std::string ShapeId::toString() const {
    return _key->toString();
}

bool ShapeId::isValid() const {
    return _key != nullptr;
};

ShapeId ShapeId::invalidId(){
    return ShapeId(nullptr, nullptr);
};

std::size_t ShapeIdHasher::operator()(ShapeId const& id) const noexcept {
    return id._key->hash();
}; 
