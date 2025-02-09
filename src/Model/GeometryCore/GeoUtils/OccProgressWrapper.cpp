
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

#include "OccProgressWrapper.hpp"

IMPLEMENT_STANDARD_RTTIEXT(OccProgressWrapper, Message_ProgressIndicator);

OccProgressWrapper::OccProgressWrapper(
    const IEventSubject& aSubject, 
    const std::string& aInitLabel) :
    _subject(aSubject),
    _progressEvent(ProgressEvent(aInitLabel, 0)){};

void OccProgressWrapper::publishProgress(const std::string& aLabel, int progress){
    _progressEvent.label = aLabel;
    _progressEvent.value = progress;
    _subject.publishEvent(_progressEvent);
}



void OccProgressWrapper::Show(
    const Message_ProgressScope& aScope,
    const Standard_Boolean force){
        std::stringstream message = getProgressMessage(aScope);
        int progress = 100 * GetPosition();
        _progressEvent.label = message.str();
        _progressEvent.value = progress;
        _subject.publishEvent(_progressEvent);
    }

std::stringstream OccProgressWrapper::getProgressMessage(
    const Message_ProgressScope& aScope)
    {
        std::stringstream message;
        message.setf(std::ios::fixed, std::ios::floatfield);
        message.precision(0);

        for(const Message_ProgressScope* parentScope = &aScope;
            parentScope;
            parentScope=parentScope->Parent())
        {
            if(!parentScope->Name()){
                continue;
            }
            Standard_Real progressVal = parentScope->Value();
            if (parentScope->IsInfinite())
            {
                if (Precision::IsInfinite(progressVal)) {
                    message << "finished";
                } else {
                    message << progressVal;
                }
            } else {
                message << progressVal << " / " << parentScope->MaxValue();
            }
        }
    return message;
}