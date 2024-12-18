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

#include "AddSizingCommand.hpp"
#include "TreeStructure.hpp"
#include "GeometrySignalSender.hpp"
#include "DocItemTypes.hpp"

AddSizingCommand::AddSizingCommand( GeometrySignalSender* aSignalSender,
                                    TreeStructure* aTreeStructure)
    :
    Command(),
    _signalSender(aSignalSender),
    _treeStructure(aTreeStructure){}

void AddSizingCommand::execute(){
    std::vector<int> shapesTags = _signalSender->getSelectedShapes();
    IVtk_SelectionMode selectionType = _signalSender->getSelectionType();

    _treeItem = _treeStructure->addElementSizingItem(shapesTags, selectionType);
}

void AddSizingCommand::undo(){
    _treeStructure->removeSubItem(_treeItem);
    delete _treeItem;
    //TODO: remove shapes from geometry
}