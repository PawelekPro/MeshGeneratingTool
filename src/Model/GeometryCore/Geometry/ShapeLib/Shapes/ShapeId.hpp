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

#ifndef SHAPEID_HPP
#define SHAPEID_HPP

#include "ShapeTypes.hpp"
#include <string>
#include <cstdint>
#include <memory>

class ShapeId {

    public:
    ShapeId(
        uint64_t id,
        const ShapeType& type,
        std::shared_ptr<const ShapeId> parentId = nullptr
    );
    virtual ~ShapeId() = default;

    virtual bool operator==(const ShapeId& other) const;
    virtual bool operator<(const ShapeId& other) const;

    virtual const ShapeType shapeType() const;
    virtual std::shared_ptr<const ShapeId> parentId() const;
    virtual std::string toString() const;
    virtual const uint64_t toInt() const;

    protected:
    const uint64_t _id;
    const ShapeType _shapeType;
    std::shared_ptr<const ShapeId> _parentId;
};

#endif