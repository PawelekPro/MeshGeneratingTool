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

#include "EventProgressIndicator.hpp"
#include "ModelEvents.hpp"

EventProgressIndicator::EventProgressIndicator(
    const EventPublisher& aModelSubject
) : _subject(aModelSubject){}

void EventProgressIndicator::begin(
    const std::string& aMessage,
    int aMaxProgress
) const {
    ProgressEvent event(aMessage, aMaxProgress, ProgressState::Begin);
    _subject.publishEvent(event);
}
void EventProgressIndicator::progress(
    const std::string& aMessage, 
    int aProgress
) const {
    ProgressEvent event(aMessage, aProgress, ProgressState::Progress);
    _subject.publishEvent(event);
}

void EventProgressIndicator::finish(
    const std::string& aMessage) const {
    ProgressEvent event(aMessage, 100, ProgressState::Finish);
    _subject.publishEvent(event);
}