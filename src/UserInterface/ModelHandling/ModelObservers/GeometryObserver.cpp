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

#include "GeometryObserver.hpp"
#include "ShapeEvents.hpp"
#include <TopoDS_Shape.hxx>
#include <iostream>

void GeometryObserver::visit(const NewShapesEvent& aEvent) const {
    const std::map<std::string, TopoDS_Shape>& shapes = aEvent.newShapes;
    std::cout << "Imported: " << shapes.size() << " shapes!" << std::endl;
};

void GeometryObserver::visit(const ShapesRemovedEvent&) const {

};