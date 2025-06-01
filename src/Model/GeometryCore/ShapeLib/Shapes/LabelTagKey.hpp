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

#ifndef LABELTAGKEY_HPP 
#define LABELTAGKEY_HPP 

#include <string>
#include <memory>
#include "ShapeKeyBase.hpp"
#include <string>
#include <tuple>

class LabelTagKey : public ShapeKeyBase<LabelTagKey> {
public:
    LabelTagKey(size_t label, size_t parent)
        : _labelTag(label), _parentLabelTag(parent),
          _cachedString(std::to_string(parent) + "-" + std::to_string(label)) {}

    std::size_t hash() const override {
        std::size_t h1 = std::hash<size_t>{}(_labelTag);
        std::size_t h2 = std::hash<size_t>{}(_parentLabelTag);
        return h1 ^ (h2 << 1);
    }

    std::string toString() const override {
        return _cachedString;
    }

    bool equalsImpl(const LabelTagKey& other) const;
    bool lessImpl(const LabelTagKey& other) const;

    size_t parentLabelTag() const {return _parentLabelTag;}    
    size_t labelTag() const {return _labelTag;}

    private:
    const size_t _labelTag;
    const size_t _parentLabelTag;
    const std::string _cachedString;
};

#endif // LABELTAGKEY_HPP