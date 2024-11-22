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

#ifndef REMOVETREEITEMCOMMAND_HPP
#define REMOVETREEITEMCOMMAND_HPP

#include "TreeCommand.hpp"

class RemoveTreeItemCommand : public TreeCommand{
    
    friend class TreeCommandManager;

    private:

    RemoveTreeItemCommand( TreeStructure* aTreeStrucure, TreeItem* aItemToRemove );

    ~RemoveTreeItemCommand();

    void execute() override;

    void undo() override;

    protected:
    
    TreeItem* _removedItemParent;
    TreeItem* _removedItem;
};

#endif
