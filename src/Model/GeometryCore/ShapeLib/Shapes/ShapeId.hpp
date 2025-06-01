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

#include <string>
#include <memory>
#include "ShapeKey.hpp"

class ShapeIdFactory;
class ShapeIdHasher;

class ShapeId {

    public:
    virtual ~ShapeId() = default;
    ShapeId() : ShapeId(invalidId()){}

    ShapeId(const ShapeId& other) noexcept;
    ShapeId(ShapeId&& other) noexcept;
    ShapeId& operator=(const ShapeId& other) noexcept;
    ShapeId& operator=(ShapeId&& other) noexcept;
    
    virtual bool operator==(const ShapeId& other) const;
    virtual bool operator<(const ShapeId& other) const;

    virtual std::string toString() const;

    virtual bool isValid() const;
    static ShapeId invalidId();

    protected:
    
    ShapeId(std::unique_ptr<ShapeKey> aKey);
    
    template<typename KeyType>
    KeyType const& key() const {
        auto* p = dynamic_cast<KeyType const*>(_key.get());
        if (!p) throw std::bad_cast();
        return *p;
    }
    
    private:

    std::unique_ptr<ShapeKey> _key;

    friend class ShapeIdFactory;
    friend class ShapeIdHasher;
};

struct ShapeIdHasher {
  std::size_t operator()(ShapeId const& id) const noexcept; 
};

#endif