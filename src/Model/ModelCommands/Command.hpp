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

#ifndef COMMAND_HPP
#define COMMAND_HPP

class Command {

    public:

    virtual ~Command() = default;

    virtual bool execute() = 0;
    virtual bool undo() = 0;
};

class SpyCommand : public Command{

    public:
    SpyCommand() = default;
    virtual ~SpyCommand() = default;
    
    virtual bool execute() override {_nExecute +=1; return true;};
    virtual bool undo() override {_nUndo += 1; return true;};

    int nExecute(){return _nExecute;};
    int nUndo(){return _nUndo;};

    private:
    int _nExecute = 0;
    int _nUndo = 0;

};

#endif