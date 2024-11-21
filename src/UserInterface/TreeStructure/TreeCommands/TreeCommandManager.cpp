/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#include "TreeCommandManager.hpp"

TreeCommandManager::~TreeCommandManager() {
    while (!_undoStack.isEmpty()) {
        delete _undoStack.pop();
    }

    while (!_redoStack.isEmpty()) {
        delete _redoStack.pop();
    }
}

void TreeCommandManager::executeCommand(TreeCommand* command) {
    command->execute();
    _undoStack.push(command);
    _redoStack.clear();
}

void TreeCommandManager::undo() {
    if (!_undoStack.isEmpty()) {
        TreeCommand* command = _undoStack.pop();
        command->undo();
        _redoStack.push(command);
    }
}

void TreeCommandManager::redo() {
    if (!_redoStack.isEmpty()) {
        TreeCommand* command = _redoStack.pop();
        command->execute();
        _undoStack.push(command);
    }
}

