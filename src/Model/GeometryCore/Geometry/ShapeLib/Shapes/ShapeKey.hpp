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

#ifndef SHAPEKEY_HPP
#define SHAPEKEY_HPP

#include <string>
#include <memory>

class ShapeKey {
    public:
    virtual ~ShapeKey() = default;
    ShapeKey() = default;
    
    ShapeKey(ShapeKey const&)            = delete;
    ShapeKey& operator=(ShapeKey const&) = delete;
    ShapeKey(ShapeKey&&)                 = delete;
    ShapeKey& operator=(ShapeKey&&)      = delete;

    virtual std::unique_ptr<ShapeKey> clone() const = 0;

    virtual bool equals(const ShapeKey& other) const = 0;
    virtual bool less  (const ShapeKey& other) const = 0;
    
    virtual std::size_t hash() const = 0;
    virtual std::string toString() const = 0;
};

#endif