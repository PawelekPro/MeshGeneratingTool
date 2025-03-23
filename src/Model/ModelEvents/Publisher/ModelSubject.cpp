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

#include "ModelSubject.hpp"
#include "EventObserver.hpp"
#include "Event.hpp"
#include <algorithm>

void ModelSubject::publishEvent(const Event& aModelEvent) const {
    notifyObservers(aModelEvent);
}

void ModelSubject::attachObserver(std::shared_ptr<EventObserver> aObserver){
    _observers.push_back(aObserver);
}

void ModelSubject::detachObserver(std::shared_ptr<EventObserver> aObserver){
    _observers.erase(
        std::remove(_observers.begin(), _observers.end(), aObserver),
        _observers.end());
}

void ModelSubject::notifyObservers(const Event& aEvent) const{
    std::for_each(
        _observers.begin(), 
        _observers.end(), 
        [&aEvent](const auto observer){observer->notify(aEvent);}
    );
}