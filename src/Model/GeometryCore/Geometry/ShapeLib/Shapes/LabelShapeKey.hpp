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

#ifndef LabelShapeKey_HPP
#define LabelShapeKey_HPP

#include "ShapeKey.hpp"
#include <TDF_Label.hxx>
#include <TCollection_AsciiString.hxx>

class LabelShapeKey : public ShapeKey {

    public:
    LabelShapeKey(const TDF_Label& aShapeLabel);
    virtual ~LabelShapeKey() = default;

    std::unique_ptr<ShapeKey> clone() const override; 
    bool equals(const ShapeKey& other) const override; 
    bool less  (const ShapeKey& other) const override; 
    
    std::size_t hash() const override;
    std::string toString() const override; 

    TDF_Label label() const;

    private:
    TDF_Label _label;
    
};

#endif