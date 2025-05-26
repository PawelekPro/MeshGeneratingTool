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

#include "CommandStack.hpp"
#include "Command.hpp"

void CommandStack::execute(std::unique_ptr<Command> aCommand) {
    if (!aCommand) return;
    
    bool executed = aCommand->execute();
    if (executed) {
        _undoStack.push(std::move(aCommand));
        while (!_redoStack.empty()) {
            _redoStack.pop();
        }
    }
}

void CommandStack::undo() {
    if (_undoStack.empty()) return;
    
    std::unique_ptr<Command> command = std::move(_undoStack.top());
    _undoStack.pop();
    
    bool undone = command->undo();
    if (undone) {
        _redoStack.push(std::move(command));
    }
}

void CommandStack::redo() {
    if (_redoStack.empty()) return;
    
    std::unique_ptr<Command> command = std::move(_redoStack.top());
    _redoStack.pop();
    
    bool executed = command->execute();
    if (executed) {
        _undoStack.push(std::move(command));
    }
}
