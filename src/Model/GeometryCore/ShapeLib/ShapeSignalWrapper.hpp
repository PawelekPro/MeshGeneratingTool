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

#ifndef SHAPESIGNALWRAPPER_HPP
#define SHAPESIGNALWRAPPER_HPP

#include "MessageBus.hpp"
#include "ShapeId.hpp"
#include "ShapeCoreObserver.hpp"

class ShapeSignalWrapper : public ShapeCoreObserver{

    public:
    ShapeSignalWrapper(MessageBus& aMessageBus);

    void onShapeAdded(std::shared_ptr<ShapeKey>) override;
    void onShapeRemoved(std::shared_ptr<ShapeKey>) override;

    void onAssemblyAdded(std::shared_ptr<ShapeKey>) override;
    void onAssemblyRemoved(std::shared_ptr<ShapeKey>) override;
    
    void publishShapeAddedEvent(const ShapeId& aShapeId);
    void publishShapeRemovedEvent(const ShapeId& aShapeId);

    void publishAssemblyAddedEvent(const ShapeId& aShapeId);
    void publishAssemblyRemovedEvent(const ShapeId& aShapeId);
    
    private:
    MessageBus& _eventBus;
};

#endif