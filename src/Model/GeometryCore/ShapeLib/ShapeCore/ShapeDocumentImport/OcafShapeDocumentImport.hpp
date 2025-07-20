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

#ifndef OCAFSHAPEDOCUMENTIMPORTER_HPP
#define OCAFSHAPEDOCUMENTIMPORTER_HPP

#include "OcafShapeRegistry.hpp"

#include <Standard_Handle.hxx>
#include <TDocStd_Document.hxx>

class OcafShapeDocumentImporter{
    public: 
    OcafShapeDocumentImporter(
        std::shared_ptr<ShapeRegistry> aDestRegistry
    );
    
    bool importDocument(
        Handle(TDocStd_Document) aSourceDoc
    );
    
    protected:
    TDF_Label importAssembly(
        const TDF_Label& aSourceLabel,
        TDF_Label aDestParentLabel
    );

    TDF_Label importPart(
        const TDF_Label& aSourceLabel,
        TDF_Label aDestParentLabel
    );

    ShapeImportData extractShape(
        const TDF_Label& aLabel
    );

    void importLabelSequence(
        const TDF_LabelSequence& aLabelSequence, 
        TDF_Label aDestParentLabel
    );

    private:
        std::shared_ptr<ShapeRegistry> _destRegistry;
        Handle(TDocStd_Document) _sourceDocument;
        Handle(XCAFDoc_ShapeTool) _sourceShapeTool;
        Handle(XCAFDoc_ColorTool) _sourceColorTool;
};


#endif