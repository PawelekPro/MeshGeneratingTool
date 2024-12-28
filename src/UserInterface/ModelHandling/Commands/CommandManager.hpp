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

#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <QStack>
#include <QList>
#include <QObject>

#include "Command.hpp"


/**
 * Main command handler class. All undoable actions should be executed using executeCommand method.
 * Upon execution they will be added to undoStack. To implement new undoable action derive from 
 * Command class. 
 */
class CommandManager : public QObject{
    Q_OBJECT

    public:
    CommandManager(QObject* aParent);
    ~CommandManager();

    /**
    * @brief calls command's execute method and adds it to the undoStack.
    * @param command command object which execute() will be called.
    */ 
    void executeCommand(Command* command);
    
    /**
    * @brief Undos first command object in the undoStack by calling its undo(). Removes the executed
    * command from the undoStack.
    */
    void undo();
    
    /**
    * @brief Redos first command object in the redoStack by calling its execute(). Removes the executed
    * command from the redoStack.
    */
    void redo();

    private:

    QStack<Command*> _undoStack;
    QStack<Command*> _redoStack;
};

#endif
