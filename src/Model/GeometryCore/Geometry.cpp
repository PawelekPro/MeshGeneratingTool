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

#include "Geometry.hpp"
#include "ShapeEvents.hpp"
#include "ImportSTEPCommand.hpp"

Geometry::Geometry(
    CommandStack& aCommandStack, 
    const ModelSubject& aModelSubject
) : 
_commandStack(aCommandStack),
_subject(aModelSubject),
_shapeCore(OcafShapeCore()),
_shapeService(aModelSubject, _shapeCore),
_commandFactory(_subject, _shapeService){}


void Geometry::importSTEP(const std::string& aFilePath){
    std::unique_ptr<ImportSTEPCommand> importCommand = 
        _commandFactory.importSTEP(aFilePath);
    if (importCommand) {
        _commandStack.execute(std::move(importCommand));
    }
}