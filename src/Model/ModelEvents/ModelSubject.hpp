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

#ifndef MODELSUBJECT_HPP
#define MODELSUBJECT_HPP

#include <vector>
#include <memory>

class Event;
class EventObserver;

class ModelSubject {

    public:
        ModelSubject() = default;
        ~ModelSubject() = default;

        void publishEvent(const Event& aEvent) const;

        void attachObserver(std::shared_ptr<EventObserver>);
        void detachObserver(std::shared_ptr<EventObserver>);

    private:
        void notifyObservers(const Event& aEvent) const;
        std::vector<std::shared_ptr<EventObserver>> _observers;

};



#endif