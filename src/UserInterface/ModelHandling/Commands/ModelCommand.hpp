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

#ifndef MODELCOMMAND_HPP
#define MODELCOMMAND_HPP

#include <memory>

class ModelInterface;

class ModelCommand{

    friend class ModelCommandManager;

    public:

    ModelCommand(std::shared_ptr<ModelInterface> aModelInterface) : _modelInterface(aModelInterface){};
    virtual ~ModelCommand() = default;
    protected:

    virtual void execute() = 0;
    virtual void undo() = 0;

    std::shared_ptr<ModelInterface> _modelInterface;

};

#endif
