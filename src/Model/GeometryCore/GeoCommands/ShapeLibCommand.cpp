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

#include "ShapeLibCommand.hpp"
#include <iostream>
#include "ShapeCore.hpp"

ShapeLibCommand::ShapeLibCommand(ShapeCore& aShapeCore)
 : _shapeCore(aShapeCore){};

bool ShapeLibCommand::execute() {
    if (!_shapeCore.openCommand()) {
        return false;
    }

    if (executeAction()) {
        _commandId = _shapeCore.commitCommand();
        return true;
    }

    _shapeCore.abortCommand();
    return false;
}

bool ShapeLibCommand::undo() {
    if (_commandId == _shapeCore.nextUndoId()){
        return _shapeCore.undo();
    } else {
        std::cerr << "Undo command id is out of sync with ShapeCore.";
        return false;
    }
}