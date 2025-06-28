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
#include <Standard_Handle.hxx>

#include "ShapeSignalsPublisher.hpp"
#include "ShapeMap.hpp"
#include "ShapeKey.hpp"

class ShapeCoreObserver;
class TDocStd_Document;
class ShapeCore {

    public:
    virtual ~ShapeCore() = default;

    virtual std::shared_ptr<ShapeKey> registerNewFreeShape(
        const TopoDS_Shape& Shape
    ) = 0;

    virtual bool removeShape(
        std::shared_ptr<ShapeKey> aShapeId
    ) = 0;

    virtual bool updateShape(
        const std::pair<std::shared_ptr<ShapeKey>, TopoDS_Shape>& aUpdatedShape
    ) = 0;
   
    virtual bool openCommand() = 0;
    virtual bool commitCommand() = 0;
    virtual bool abortCommand() = 0;
    virtual bool undo() = 0;
    virtual bool redo() = 0;

    virtual bool write(const std::string& aSavePath) const = 0;

    virtual std::shared_ptr<const ShapeMap> shapeMap() const;
    virtual void attachObserver(std::shared_ptr<ShapeCoreObserver> aObserver);

    virtual bool importDocument(Handle(TDocStd_Document) aDoc) = 0;

    protected:

    ShapeSignalsPublisher _publisher;
    std::shared_ptr<ShapeMap> _shapeMap;
};

#endif