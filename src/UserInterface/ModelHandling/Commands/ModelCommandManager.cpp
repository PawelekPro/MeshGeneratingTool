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

#include "ModelCommandManger.hpp"


void ModelCommandManager::executeCommand(ModelCommand* command) {
    if (!command) return;

    command->execute();

    _undoStack.push(command);
    while (!_redoStack.isEmpty()) {
        delete _redoStack.pop();
    }
}

void ModelCommandManager::undo() {
    if (_undoStack.isEmpty()) return;

    ModelCommand* command = _undoStack.pop();
    command->undo();
    _redoStack.push(command);
}

void ModelCommandManager::redo() {
    if (_redoStack.isEmpty()) return;
    ModelCommand* command = _redoStack.pop();

    command->execute();
    _undoStack.push(command);
}