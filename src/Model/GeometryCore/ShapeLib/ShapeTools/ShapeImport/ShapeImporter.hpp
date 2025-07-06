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

#ifndef SHAPEIMPORTER_HPP
#define SHAPEIMPORTER_HPP

#include "ProgressIndicator.hpp"
#include "FileUtils.hpp"

#include <Standard_Handle.hxx>
#include <XCAFApp_Application.hxx>
#include <string>

class TDocStd_Document;
class ShapeImporter {
    
    public:
    virtual ~ShapeImporter() = default;

    virtual Handle(TDocStd_Document) import(
        const std::string& aFilePath, 
        const ProgressIndicator& aProgressIndicator = IdleProgressIndicator()
    ) const = 0;

    Handle(TDocStd_Document) initDocument() const;
};

#endif
