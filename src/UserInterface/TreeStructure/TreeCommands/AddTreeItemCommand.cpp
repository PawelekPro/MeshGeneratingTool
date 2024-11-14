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

#include "AddTreeItemCommand.hpp"

AddTreeItemCommand::AddTreeItemCommand(TreeStructure* aTreeStructure,
                        TreeItemFactory* aTreeItemFactory) :
                        TreeCommand(aTreeStructure),
                        _treeItemFactory(aTreeItemFactory)
                        {}

void AddTreeItemCommand::execute(){

    //TODO: add logic that brings back the removed item. I am not sure
    //      where to store the item. When we remove it in TreeStructure
    //      it should still live as long as it is in the CommandHistory
    //      so we can bring it up using TreeCommands _treeItem pointer.
    //      Think of some storage for the item or maybe rethink the pointer
    //      usage and just store copy of item in command.
}

void AddTreeItemCommand::undo(){
    _treeStructure->removeSubItem(_treeItem);
}


