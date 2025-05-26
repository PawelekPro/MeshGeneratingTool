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

#ifndef BASECOMMAND_HPP
#define BASECOMMAND_HPP

class BaseCommand {

    public:

    virtual ~BaseCommand() = default;

    virtual bool execute() = 0;
    virtual bool undo() = 0;
};

class SpyCommand : public BaseCommand{

    public:
    SpyCommand() = default;
    virtual ~SpyCommand() = default;
    
    virtual bool execute() override {_nExecuted +=1; return true;};
    virtual bool undo() override {_nUndone += 1; return true;};

    int nExecute(){return _nExecuted;};
    int nUndo(){return _nUndone;};

    private:
    int _nExecuted = 0;
    int _nUndone = 0;

};

#endif