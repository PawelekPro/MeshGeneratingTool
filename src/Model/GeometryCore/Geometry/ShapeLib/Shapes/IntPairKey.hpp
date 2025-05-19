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

#ifndef IntPairKey_HPP 
#define IntPairKey_HPP 

#include <string>
#include <memory>
#include "ShapeKey.hpp"

class IntPairKey : public ShapeKey {
    public:
    virtual ~IntPairKey() = default;
    IntPairKey(size_t aLabelTag, size_t aParentLabelTag);
    
    virtual std::unique_ptr<ShapeKey> clone() const override;

    virtual bool equals(const ShapeKey& other) const override;
    virtual bool less  (const ShapeKey& ther) const override;
    
    virtual std::size_t hash() const override;
    virtual std::string toString() const override;

    size_t labelTag() const {return _labelTag;}
    size_t tNamingId() const {return _parentLabelTag;}

    private:
    const size_t _labelTag;
    const size_t _parentLabelTag;
    std::string _cachedString;
};

#endif