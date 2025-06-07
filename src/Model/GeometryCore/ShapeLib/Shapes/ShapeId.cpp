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
    std::shared_ptr<ShapeKey> aKey
) : _key(aKey){}

ShapeId::ShapeId(const ShapeId& aOther) noexcept
    : _key(aOther._key){}

ShapeId::ShapeId(ShapeId&& aOther) noexcept
    : _key(std::move(aOther._key))
{}

ShapeId& ShapeId::operator=(const ShapeId& aOther) noexcept{
    if (this != &aOther) {
      _key      = aOther._key;
    }
    return *this;
}   

ShapeId& ShapeId::operator=(ShapeId&& aOther) noexcept {
    if (this != &aOther) {
      _key      = aOther._key;
    }
    return *this;
}

bool ShapeId::operator==(const ShapeId& other) const {
    if (!_key || !other._key) return _key == other._key;
    return *_key == *other._key;
}

bool ShapeId::operator<(const ShapeId& other) const {
    if (!_key || !other._key) return _key < other._key;
    return *_key < *other._key;
}

std::string ShapeId::toString() const {
    return _key->toString();
}

bool ShapeId::isValid() const {
    return _key != nullptr;
};

ShapeId ShapeId::invalidId(){
    return ShapeId(nullptr);
};

std::size_t ShapeIdHasher::operator()(ShapeId const& id) const noexcept {
    return id._key->hash();
};
