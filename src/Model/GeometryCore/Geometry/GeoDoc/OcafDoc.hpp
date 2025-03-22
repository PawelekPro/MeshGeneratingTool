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
#ifndef OCAFDOC_HPP
#define OCAFDOC_HPP

#include <string>
#include <optional>

#include <TDocStd_Document.hxx>
#include <TDF_Label.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Trsf.hxx>

#include "IEventSubject.hpp"

class OcafImporter;

class OcafDoc{

    public: 
        virtual ~OcafDoc() = default;     
  
        virtual std::optional<TopoDS_Shape> getShape(const TDF_Label& aLabel) const = 0;
        virtual std::optional<TDF_Label> getLabel(const TopoDS_Shape& aShape) const = 0;
        
        virtual std::vector<TDF_Label> importShapes(const OcafImporter& aImporter) = 0;
        

        virtual void undo() = 0;
        virtual bool save(const std::string& filePath) const = 0;
    };

#endif