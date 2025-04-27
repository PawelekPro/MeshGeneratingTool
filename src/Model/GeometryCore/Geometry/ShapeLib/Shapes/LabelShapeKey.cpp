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

#include "LabelShapeKey.hpp"
#include <TDF_Tool.hxx>

LabelShapeKey::LabelShapeKey(const TDF_Label& aShapeLabel)
    : ShapeKey(), _label(aShapeLabel){}

std::unique_ptr<ShapeKey> LabelShapeKey::clone() const {
    return std::make_unique<LabelShapeKey>(_label);
} 

bool LabelShapeKey::equals(const ShapeKey& other) const {
    if (auto* p = dynamic_cast<LabelShapeKey const*>(&other))
      return p->_label == _label;
    return false;
}

bool LabelShapeKey::less(const ShapeKey& other) const {
    return this->toString() < other.toString();
}

std::size_t LabelShapeKey::hash() const {
    return std::hash<TDF_Label>{}(_label);
}

std::string LabelShapeKey::toString() const {
    TCollection_AsciiString entry;
    TDF_Tool::Entry(_label, entry);
    return entry.ToCString();
}

TDF_Label LabelShapeKey::label() const { return _label; }