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

#include "RemoveTreeItemCommand.hpp"
#include "TreeStructure.hpp"

RemoveTreeItemCommand::RemoveTreeItemCommand( TreeStructure* aTreeStructure,  TreeItem* aItemToRemove ) 
                                            : TreeCommand(aTreeStructure),
                                            _removedItem(aItemToRemove),
                                            _removedItemParent(dynamic_cast<TreeItem*>(aItemToRemove->parent()))
{}

void RemoveTreeItemCommand::execute(){
    _treeStructure->removeSubItem(_removedItem);
}

void RemoveTreeItemCommand::undo(){
    _treeStructure->addExistingItem(_removedItem, _removedItemParent);
}

RemoveTreeItemCommand::~RemoveTreeItemCommand(){
    if(_removedItem){
        delete _removedItem;
    }
}