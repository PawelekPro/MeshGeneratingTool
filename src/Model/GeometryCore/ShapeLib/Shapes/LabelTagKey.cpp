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

#include "LabelTagKey.hpp"



LabelTagKey::LabelTagKey(
    size_t aLabelTag, 
    size_t aParentLabelTag
) : _labelTag(aLabelTag), 
    _parentLabelTag(aParentLabelTag),
    _cachedString(std::to_string(aParentLabelTag) + "-" + std::to_string(aLabelTag))
    {}

bool LabelTagKey::equalsImpl(const LabelTagKey& other) const {
    return  _labelTag == other._labelTag &&
            _parentLabelTag == other._parentLabelTag;
}

bool LabelTagKey::lessImpl(const LabelTagKey& other) const {
    return std::tie(_parentLabelTag, _labelTag) < std::tie(other._parentLabelTag, other._labelTag);
}

std::size_t LabelTagKey::hash() const {
        std::size_t h1 = std::hash<size_t>{}(_labelTag);
        std::size_t h2 = std::hash<size_t>{}(_parentLabelTag);
        return h1 ^ (h2 << 1);
}


std::string LabelTagKey::toString() const {
    return std::to_string(parentLabelTag) + "-" + std::to_string(labelTag);
}