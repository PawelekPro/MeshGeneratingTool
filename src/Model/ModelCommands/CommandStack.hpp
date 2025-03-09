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

#ifndef COMMANDSTACK_HPP
#define COMMANDSTACK_HPP

#include <memory>
#include <stack>

class Command;
class CommandStack {

    public:
    CommandStack() = default;
    ~CommandStack() = default;

    void execute(std::unique_ptr<Command> aCommand);
    void undo();
    void redo();
   
    size_t undoStackLength(){return _undoStack.size();};
    size_t redoStackLength(){return _redoStack.size();}; 
    
    private:
    
    std::stack<std::unique_ptr<Command>> _undoStack;
    std::stack<std::unique_ptr<Command>> _redoStack;
};

#endif