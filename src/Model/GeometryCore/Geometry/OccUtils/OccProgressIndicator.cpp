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
#include "OccProgressIndicator.hpp"

IMPLEMENT_STANDARD_RTTIEXT(OccProgressIndicator, Message_ProgressIndicator);

OccProgressIndicator::OccProgressIndicator(
    const ProgressIndicator& aProgressIndicator
) : _progressIndicator(aProgressIndicator){};

void OccProgressIndicator::Show(
    const Message_ProgressScope& aScope,
    const Standard_Boolean force
){
    std::stringstream message = progressMessage(aScope);
    int progress = 100 * GetPosition();
    _progressIndicator.progress(message.str(), progress);
}

std::stringstream OccProgressIndicator::progressMessage(
    const Message_ProgressScope& aScope
)
{
    std::stringstream message;
    message.setf(std::ios::fixed, std::ios::floatfield);
    message.precision(0);

    std::vector<std::string> scopeMessages;

    for (const Message_ProgressScope* parentScope = &aScope;
         parentScope;
         parentScope = parentScope->Parent())
    {
        if (!parentScope->Name()) {
            continue;
        }

        std::stringstream scopeMessage;
        scopeMessage << parentScope->Name() << ": ";
        
        Standard_Real progressVal = parentScope->Value();
        if (parentScope->IsInfinite())
        {
            if (Precision::IsInfinite(progressVal)) {
                scopeMessage << "finished";
            } else {
                scopeMessage << progressVal;
            }
        }
        else
        {
            scopeMessage << progressVal << " / " << parentScope->MaxValue();
        }

        scopeMessages.push_back(scopeMessage.str());
    }

    std::reverse(scopeMessages.begin(), scopeMessages.end());

    for (size_t i = 0; i < scopeMessages.size(); ++i)
    {
        if (i > 0) {
            message << " -> ";
        }
        message << scopeMessages[i];
    }

    return message;
}