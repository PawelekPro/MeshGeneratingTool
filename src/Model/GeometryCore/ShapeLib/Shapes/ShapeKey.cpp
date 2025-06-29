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

#include "ShapeKey.hpp"
#include <sstream>
#include <functional>
#include <numeric>

ShapeKey::ShapeKey(std::vector<int> aShapeTreePath)
: _shapeTreePath(std::move(aShapeTreePath)),
  _cachedString([this](){
      std::ostringstream oss;
      for (size_t i = 0; i < _shapeTreePath.size(); ++i) {
          oss << _shapeTreePath[i];
          if (i + 1 < _shapeTreePath.size()) oss << ":";
      }
      return oss.str();
  }()) {}

bool ShapeKey::operator==(const ShapeKey& other) const {
    return _shapeTreePath == other._shapeTreePath;
}

bool ShapeKey::operator<(const ShapeKey& other) const {
    return _shapeTreePath < other._shapeTreePath;
}

std::size_t ShapeKey::hash() const {
    std::size_t seed = 0;
    for (int val : _shapeTreePath) {
        seed ^= std::hash<int>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::string ShapeKey::toString() const {
    return _cachedString;
}

