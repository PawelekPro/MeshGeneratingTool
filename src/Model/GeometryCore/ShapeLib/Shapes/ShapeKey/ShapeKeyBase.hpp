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

#ifndef SHAPEKEYBASE_HPP
#define SHAPEKEYBASE_HPP

#include "ShapeKeyBase.hpp"
#include "ShapeKeyConcept.hpp"
#include <type_traits>

template <typename Derived>
requires ShapeKeyConcept<Derived>
class ShapeKeyBase : public ShapeKey {
public:
    bool equals(const ShapeKey& other) const final {
        if (auto* p = dynamic_cast<const Derived*>(&other)) {
            return static_cast<const Derived*>(this)->equalsImpl(*p);
        }
        return false;
    }

    bool less(const ShapeKey& other) const final {
        if (auto* p = dynamic_cast<const Derived*>(&other)) {
            return static_cast<const Derived*>(this)->lessImpl(*p);
        }
        return false;
    }
};

#endif // SHAPEKEYCRTP_HPP
