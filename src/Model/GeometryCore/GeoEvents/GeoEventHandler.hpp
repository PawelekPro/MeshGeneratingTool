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

#ifndef GEOEVENTHANDLER_HPP
#define GEOEVENTHANDLER_HPP

#include <vector>

class IEventSubject;
class ShapeId;

class GeoEventHandler {

    using ShapeIds = std::vector<ShapeId>;    

    public:
    GeoEventHandler(const IEventSubject& aSubject);
    
    void shapesAdded(const ShapeIds& aShapeIdsVec);
    void shapesRemoved(const ShapeIds& aShapeIdsVec);
    void shapesBRepEdited(const ShapeIds& aShapeIdsVec);
    void shapesAttrEdited(const ShapeIds& aShapeIdsVec);

    private:
    const IEventSubject& _subject;

};

#endif