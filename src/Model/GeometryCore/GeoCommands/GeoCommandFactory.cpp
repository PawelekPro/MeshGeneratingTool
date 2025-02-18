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

#include "GeoCommandFactory.hpp"

#include "RenameCommand.hpp"
#include "ImportCommand.hpp"
#include "ScaleShapeCommand.hpp"


GeoCommandFactory::GeoCommandFactory(
        ShapeMap& aShapeMap,
        ShapeStateManager& aShapeStateManager,
        const IEventSubject& aSubject) : 
        _shapeMap(aShapeMap),
        _shapeState(aShapeStateManager),
        _subject(aSubject){};



const ImportCommand GeoCommandFactory::importCommand(
    const std::string& aFilePath){

    };


const ScaleShapeCommand GeoCommandFactory::scaleShapeCommand(
    const ShapeId& aShapeId, double aScale){

    };

const RenameCommand GeoCommandFactory::renameCommand(
    const ShapeId& aShapeId, const std::string& aNewName){


    };