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

#ifndef SHAPECORE_HPP
#define SHAPECORE_HPP

#include <memory>
#include <vector>
#include <boost/signals2.hpp>

#include "ShapeEventsPublisher.hpp"
#include "ShapeMap.hpp"
#include "ShapeId.hpp"

class ShapeCore {

    public:
    virtual ~ShapeCore() = default;

    virtual const ShapeId registerNewShape(
        const TopoDS_Shape& Shape
    ) = 0;

    virtual bool removeShape(
        const ShapeId& aShapeId
    ) = 0;

    virtual bool updateShape(
        const std::pair<ShapeId, TopoDS_Shape>& aUpdatedShape
    ) = 0;
   
    virtual bool openCommand() = 0;
    virtual bool commitCommand() = 0;
    virtual bool abortCommand() = 0;
    virtual bool undo() = 0;

    virtual std::shared_ptr<const ShapeMap> shapeMap() const = 0;

    boost::signals2::connection connectShapeAdded(
        const std::function<void(const ShapeId&)>& slot
    );
    boost::signals2::connection connectShapeRemoved(
        const std::function<void(const ShapeId&)>& slot
    );
    boost::signals2::connection connectShapeModified(
        const std::function<void(const ShapeId&)>& slot
    );

    protected:
    ShapeEventsPublisher _publisher;

};

#endif