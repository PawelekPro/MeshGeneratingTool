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

#ifndef SHAPESIGNALSPUBLISHER_HPP
#define SHAPESIGNALSPUBLISHER_HPP

#include "ShapeId.hpp"
#include "ShapeCoreObserver.hpp"
#include <boost/signals2.hpp>

class ShapeSignalsPublisher {
    public:
    using ShapeSignal = boost::signals2::signal<void(std::shared_ptr<ShapeKey>)>;

    ShapeSignalsPublisher() = default;

    void publishShapeAdded(std::shared_ptr<ShapeKey> aShapeKey) {
        onShapeAdded(aShapeKey);
    }

    void publishShapeRemoved(std::shared_ptr<ShapeKey> aShapeKey) {
        onShapeRemoved(aShapeKey);
    }

    void attachObserver(std::shared_ptr<ShapeCoreObserver> aObserver)  {
        onShapeAdded.connect(
            [weakObs = std::weak_ptr(aObserver)](std::shared_ptr<ShapeKey> id) {
                if (auto obs = weakObs.lock()) {
                    obs->onShapeAdded(id);
                }
            }
        );

        onShapeRemoved.connect(
            [weakObs = std::weak_ptr(aObserver)](std::shared_ptr<ShapeKey> id) {
                if (auto obs = weakObs.lock()) {
                    obs->onShapeRemoved(id);
                }
            }
        );
    }

private:
    ShapeSignal onShapeAdded;
    ShapeSignal onShapeRemoved;
    ShapeSignal onShapeModified;
};

#endif // SHAPEEVENTSPUBLISHER_HPP