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

#ifndef SHAPEEVENT_HPP
#define SHAPEEVENT_HPP

#include "GeoEvent.hpp"
#include <TopoDS_Shape.hxx>
#include <map>
class ShapeAddedEvent : public GeoEvent {
    public:
    ShapeAddedEvent(const ShapeId aNewShapeId) : _newShapeId(aNewShapeId){};

    void accept(BaseGeometryObserver& aEventObserver) const override {
       aEventObserver.visit(*this);
    }
    const ShapeId id(){return _newShapeId;}

    private:
    const ShapeId _newShapeId;

};

class ShapeModifiedEvent : public GeoEvent {
    public:
    ShapeModifiedEvent(const ShapeId aModifiedShapeId) 
    : _modifiedShapeId(aModifiedShapeId){};

    void accept(BaseGeometryObserver& aEventObserver) const override {
       aEventObserver.visit(*this);
    }
    const ShapeId id(){return _modifiedShapeId;}

    private:
    const ShapeId _modifiedShapeId;

};

class ShapeRemovedEvent : public GeoEvent {
    public:
    ShapeRemovedEvent(const ShapeId aRemovedShapeId)
    : _removedShapeId(aRemovedShapeId){};

    void accept(BaseGeometryObserver& aEventObserver) const override {
       aEventObserver.visit(*this);
    }
    const ShapeId id(){return _removedShapeId;}

    private:
    const ShapeId _removedShapeId;

};

#endif
