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

#ifndef TREECOMMAND_HPP
#define TREECOMMAND_HPP

#include "TreeItemTypes.hpp"
#include "TreeItem.hpp"

class TreeCommanManager;
class TreeStructure;
class TreeCommand{

    friend class TreeCommandManager;

    public:

    TreeCommand(TreeStructure* aTreeStructure);

    protected:

    virtual void execute() = 0;
    virtual void undo() = 0;

    TreeStructure* _treeStructure;
};

#endif