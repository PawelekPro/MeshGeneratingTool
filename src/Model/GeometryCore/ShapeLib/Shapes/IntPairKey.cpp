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

#include "IntPairKey.hpp"

IntPairKey::IntPairKey(size_t aLabelTag, size_t aParentLabelTag)
    : ShapeKey(), _labelTag(aLabelTag), _parentLabelTag(aParentLabelTag){}

std::unique_ptr<ShapeKey> IntPairKey::clone() const {
    return std::make_unique<IntPairKey>(_labelTag, _parentLabelTag);
} 

bool IntPairKey::equals(const ShapeKey& other) const {
    return this->toString() == other.toString();
}

bool IntPairKey::less(const ShapeKey& other) const {
    return this->toString() < other.toString();
}

std::size_t IntPairKey::hash() const {
    return 1;
    //    return std::hash<const size_t>(_parentLabelTag);
}

std::string IntPairKey::toString() const {
    return "a";
    // if (_cachedString.empty()){
    //     _cachedString = std::to_string(_labelTag) + "-" + 
    //                     std::to_string(_parentLabelTag);
    // } else {
    //     return _cachedString;
    // }
}