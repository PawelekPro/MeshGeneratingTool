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

#include "AttributeFactory.hpp"
#include "ShapeKeyAttr.hpp"
#include "OcafShapeCore.hpp"
#include "ShapeKey.hpp"

AttributeFactory::AttributeFactory(
    ShapeSignalsPublisher& aPublisher
) : _publisher(aPublisher) {}

Handle(ShapeKeyAttr) AttributeFactory::shapeKeyAttr(
    std::shared_ptr<ShapeKey> aKey
) {
    Handle(ShapeKeyAttr) attr = new ShapeKeyAttr(
        aKey->labelTag(), aKey->parentLabelTag()
    );

    attr->shapeAddedSignal().connect(
        [publisher = std::ref(_publisher)](
            Standard_Integer labelTag, 
            Standard_Integer parentLabelTag
        ) {
            auto key = std::make_shared<ShapeKey>(labelTag, parentLabelTag);
            publisher.get().publishShapeAdded(key);
        }
    );

    attr->shapeRemovedSignal().connect(
        [publisher = std::ref(_publisher)](
            Standard_Integer labelTag, 
            Standard_Integer parentLabelTag
        ) {
            auto key = std::make_shared<ShapeKey>(labelTag, parentLabelTag);
            publisher.get().publishShapeRemoved(key);
        }
    );

    
    return attr;
}