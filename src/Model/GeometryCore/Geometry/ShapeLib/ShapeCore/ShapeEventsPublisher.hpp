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

#ifndef SHAPEEVENTSPUBLISHER_HPP
#define SHAPEEVENTSPUBLISHER_HPP

#include "ShapeId.hpp"
#include <boost/signals2.hpp>

class ShapeEventsPublisher {
public:
    using ShapeSignal = boost::signals2::signal<void(const ShapeId&)>;

    ShapeEventsPublisher() = default;

    void shapeAdded(const ShapeId& aShapeId) {
        onShapeAdded(aShapeId);
    }

    void shapeRemoved(const ShapeId& aShapeId) {
        onShapeRemoved(aShapeId);
    }

    void shapeModified(const ShapeId& aShapeId) {
        onShapeModified(aShapeId);
    }

    ShapeSignal& shapeAddedSignal() { return onShapeAdded; }
    ShapeSignal& shapeRemovedSignal() { return onShapeRemoved; }
    ShapeSignal& shapeModifiedSignal() { return onShapeModified; }

private:
    ShapeSignal onShapeAdded;
    ShapeSignal onShapeRemoved;
    ShapeSignal onShapeModified;
};

#endif // SHAPEEVENTSPUBLISHER_HPP