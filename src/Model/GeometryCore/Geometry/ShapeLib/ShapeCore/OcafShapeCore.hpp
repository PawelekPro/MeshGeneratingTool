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
#include "ShapeEventsPublisher.hpp"
#include "ShapeEventsPublisher.hpp"

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

enum class DeltaType{
    Removal,
    Addition,
    Modification,
    Forget,
    Resume
};

class StdShapeMap;
class OcafShapeCore : public ShapeCore {
    
    public:
    OcafShapeCore();
    
    virtual ~OcafShapeCore() = default;

    bool registerNewShape(
        const TopoDS_Shape& Shape
    ) override;

    bool removeShape(
        const ShapeId& aShapeId
    ) override;

    bool updateShape(
        const std::pair<ShapeId, TopoDS_Shape>& aUpdatedShape
    ) override;
   
    bool openCommand() override;
    bool commitCommand() override;
    bool abortCommand() override;
    bool undo() override;

    std::shared_ptr<const ShapeMap> shapeMap() const override;
  
    bool write(const std::string& aSavePath);
    
    private:

    int reviewDelta(Handle(TDF_Delta) aDeltaList) const;
    void publishShapeEvents(DeltaType aDeltaType) const;

    std::shared_ptr<StdShapeMap> _shapeMap;
    Handle(TDocStd_Document) _document;
    Handle(XCAFDoc_ShapeTool) _shapeTool;
    Handle(XCAFDoc_ColorTool) _colorTool;

    TDF_Label _shapeLabel;
    std::set<TDF_Label> _shapeLabels;

};

#endif