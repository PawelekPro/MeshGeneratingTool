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

#include "TagIdKey.hpp"

TagIdKey::TagIdKey(size_t aLabelTag, size_t aTNamingId)
    : ShapeKey(), _labelTag(aLabelTag), _tNamingId(aTNamingId){}

std::unique_ptr<ShapeKey> TagIdKey::clone() const {
    return std::make_unique<TagIdKey>(_labelTag, _tNamingId);
} 

bool TagIdKey::equals(const ShapeKey& other) const {
    return this->toString() == other.toString();
}

bool TagIdKey::less(const ShapeKey& other) const {
    return this->toString() < other.toString();
}

std::size_t TagIdKey::hash() const {
    return 1;
    //    return std::hash<const size_t>(_tNamingId);
}

std::string TagIdKey::toString() const {
    return "a";
    // if (_cachedString.empty()){
    //     _cachedString = std::to_string(_labelTag) + "-" + 
    //                     std::to_string(_tNamingId);
    // } else {
    //     return _cachedString;
    // }
}