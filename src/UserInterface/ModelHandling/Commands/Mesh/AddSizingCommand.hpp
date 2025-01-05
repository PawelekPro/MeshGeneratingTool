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

#ifndef ADDSIZINGCOMMAND_HPP
#define ADDSIZINGCOMMAND_HPP


#include "Command.hpp"

#include <memory>
#include <QString>

class TreeStructure;
class GeometrySignalSender;
class TreeItem;

/**
 * Command for adding element sizing mesh setting. Creates an ElementSizing TreeItem upon being executed.
 */
class AddSizingCommand : public Command{

  public: 
  AddSizingCommand( GeometrySignalSender* aSignalSender,
                    TreeStructure* aTreeStructure
                  );

  /**
  * @brief Command's execute method - creates an ElementSizing TreeItem in TreeStrucutre and keeps its pointer.
  */
  void execute() override;
  /**
  * @brief Command's undo method - removes object's _treeItem from the TreeStructure and deletes it.
  */
  void undo() override;

  private:

  GeometrySignalSender* _signalSender;
  TreeStructure* _treeStructure;
  TreeItem* _treeItem;
};

#endif
