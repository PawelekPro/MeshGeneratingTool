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

#include <cstdint>
#include "ShapeFactory.hpp"

class ShapeId {
    friend class ShapeFactory;
    
    public:
    
    bool operator==(const ShapeId& aId){
         return aId._id == _id;
    }
    
    bool operator>(const ShapeId& aId){
        return _id > aId._id;
    }
    
    private:
    explicit ShapeId(const uint64_t& aId) : _id(aId) {};
        uint64_t _id;
};




#endif