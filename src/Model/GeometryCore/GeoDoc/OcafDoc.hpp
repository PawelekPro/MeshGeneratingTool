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

#include <TDocStd_Document.hxx>
#include <TDF_Label.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Trsf.hxx>

#include "GeoShape.hpp"

class OcafDoc{

    public: 
        OcafDoc();

        bool importSTEP(const std::string& aFilePath);

        void undo();
        
        std::vector<TopoDS_Shape> getAllShapes() const;

        std::string getShapeName(const TopoDS_Shape& aShape) const;
        bool saveAsXml(const std::string& filePath) const;

    private:
        Handle(TDocStd_Document) _document;
        Handle(XCAFDoc_ShapeTool) _shapeTool;
        Handle(XCAFDoc_ColorTool) _colorTool;
};

#endif