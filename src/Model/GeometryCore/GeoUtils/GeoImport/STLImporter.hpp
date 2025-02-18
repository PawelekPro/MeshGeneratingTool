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

#ifndef STLOCAFIMPORTER_HPP
#define STLOCAFIMPORTER_HPP

#include "OcafImporter.hpp"
#include <Standard_Handle.hxx>
#include "OccProgressWrapper.hpp"
#include <TopoDS_Shape.hxx>
#include <Poly_Triangulation.hxx>

class STLOcafImporter : public OcafImporter {

    public:
    bool importToDocument(
            const std::string& aFilePath, 
            Handle(TDocStd_Document) aDestDoc
    ) override;

    STLOcafImporter(const IEventSubject& aModelSubject);
    ~STLOcafImporter() = default;

    private:
    bool transferShape(const TopoDS_Shape& aShape, Handle(TDocStd_Document) aDestDoc);
    TopoDS_Shape extractShells(const TopoDS_Shape& aShape);
    TopoDS_Shape sewShape(Handle(Poly_Triangulation) aTriangulation);
    OccProgressWrapper _progress;

};


#endif