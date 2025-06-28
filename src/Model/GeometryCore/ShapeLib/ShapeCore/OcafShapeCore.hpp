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

#ifndef OCAFSHAPECORE_HPP
#define OCAFSHAPECORE_HPP

#include "ShapeCore.hpp"
#include "ShapeSignalsPublisher.hpp"
#include "ShapeKeyAttr.hpp"
#include "ShapeKey.hpp"
#include "LabelKeyTool.hpp"
#include "AttributeFactory.hpp"

#include <string>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <gp_Trsf.hxx>
#include <TDF_Label.hxx>
#include <set>
#include <spdlog/spdlog.h>

#include <TDF_DeltaOnAddition.hxx>
#include <TDF_DeltaOnForget.hxx>
#include <TDF_DeltaOnModification.hxx>
#include <TDF_DeltaOnRemoval.hxx>
#include <TDF_DeltaOnResume.hxx>
#include <XCAFDoc_ShapeMapTool.hxx>

enum class DeltaType{
    Removal,
    Addition,
    Modification,
    Forget,
    Resume
};

class OcafShapeCore : public ShapeCore {
    
    public:
    OcafShapeCore();
    
    virtual ~OcafShapeCore() = default;

    std::shared_ptr<ShapeKey> registerNewFreeShape(
        const TopoDS_Shape& Shape
    ) override;

    bool removeShape(
        std::shared_ptr<ShapeKey> aShapeKey
    ) override;

    bool updateShape(
        const std::pair<std::shared_ptr<ShapeKey>, TopoDS_Shape>& aUpdatedShape
    ) override;
   
    bool openCommand() override;
    bool commitCommand() override;
    bool abortCommand() override;
    bool undo() override;
    bool redo() override;
   
    bool write(const std::string& aSavePath) const override;
   
    virtual bool importDocument(Handle(TDocStd_Document) aDoc) override; 
    
    private:
    void importAssemblyLabel(
        Handle(XCAFDoc_ShapeTool) aShapeTool, 
        const TDF_Label& aLabel
    );

    void importComponentLabel(
        TDF_Label& aLocalParentAssemblyLabel,
        Handle(XCAFDoc_ShapeTool) aShapeTool, 
        const TDF_Label& aLabel
    );    

    TDF_Label registerAssemblyLabel(
        Handle(XCAFDoc_ShapeTool) aShapeTool, 
        const TDF_Label& aLabel
    );

    TDF_Label registerFreeShapeLabel(
        Handle(XCAFDoc_ShapeTool) aShapeTool, 
        const TDF_Label& aLabel
    );

    TDF_Label registerSubAssemblyLabel(
        TDF_Label& aLocalParentAssemblyLabel,
        Handle(XCAFDoc_ShapeTool) aShapeTool, 
        const TDF_Label& aLabel
    );
    
    TDF_Label registerComponentLabel(
        TDF_Label& aLocalParentAssemblyLabel,
        Handle(XCAFDoc_ShapeTool) aShapeTool, 
        const TDF_Label& aLabel
    );
    
    Handle(TDocStd_Document) _document;
    Handle(XCAFDoc_ShapeTool) _shapeTool;
    Handle(XCAFDoc_ColorTool) _colorTool;

    TDF_Label _shapeLabel;
    std::unique_ptr<AttributeFactory> _attrFactory;
    std::unique_ptr<LabelKeyTool> _labelKeyTool;
};

#endif