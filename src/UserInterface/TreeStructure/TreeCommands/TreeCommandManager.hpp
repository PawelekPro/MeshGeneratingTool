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

#include <QStack>
#include <QList>

#include "TreeCommand.hpp"

class TreeCommandManager {
public:
    void executeCommand(TreeCommand* command) {
        command->execute();
        _undoStack.push(command);
        _redoStack.clear();
    }

    void undo() {
        if (!_undoStack.isEmpty()) {
            TreeCommand* command = _undoStack.pop();
            command->undo();
            _redoStack.push(command);
        }
    }

    void redo() {
        if (!_redoStack.isEmpty()) {
            TreeCommand* command = _redoStack.pop();
            command->execute();
            _undoStack.push(command);
        }
    }

private:
    QStack<TreeCommand*> _undoStack;
    QStack<TreeCommand*> _redoStack;
};