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

#include "LabelPathAttr.hpp"

#include <TDF_RelocationTable.hxx>
#include <TDF_DeltaOnRemoval.hxx>
#include <TDF_DeltaOnAddition.hxx>
#include <TDF_DeltaOnResume.hxx>
#include <iostream>

IMPLEMENT_STANDARD_RTTIEXT(LabelPathAttr, TDF_Attribute)

static const Standard_GUID GUID_ShapeIdAttr(
  "2ae4362f-ce33-4af9-909e-f009ad8516f3"
);

LabelPathAttr::LabelPathAttr()
: _labelPath() {}

LabelPathAttr::LabelPathAttr(const std::vector<int>& labelPath)
: _labelPath(labelPath) {}

const Standard_GUID& LabelPathAttr::GetID() {
  return GUID_ShapeIdAttr;
}

const Standard_GUID& LabelPathAttr::ID() const {
  return GetID();
}

Handle(TDF_Attribute) LabelPathAttr::NewEmpty() const {
  return new LabelPathAttr();
}

void LabelPathAttr::Restore(const Handle(TDF_Attribute)& from) {
  const Handle(LabelPathAttr) src =
    Handle(LabelPathAttr)::DownCast(from);
  _labelPath = src->_labelPath;
}

void LabelPathAttr::Paste(
  const Handle(TDF_Attribute)& into,
  const Handle(TDF_RelocationTable)&
) const {
  const Handle(LabelPathAttr) dst =
    Handle(LabelPathAttr)::DownCast(into);
  dst->_labelPath = _labelPath;
}

Standard_OStream& LabelPathAttr::Dump(Standard_OStream& os) const {
  os << "LabelPathAttr";
  return os;
}

void LabelPathAttr::Set(const std::vector<int>& labelPath) {
  if (_labelPath != labelPath) {
    Backup();
    _labelPath = labelPath;
  }
}

void LabelPathAttr::Get(std::vector<int>& labelPath) const {
  labelPath = _labelPath;
}

void LabelPathAttr::AfterAddition() {
  _shapeAddedSignal(_labelPath);
  std::cout << "LabelPathAttr: Attribute added" << std::endl;
}

void LabelPathAttr::BeforeRemoval() {
  _shapeRemovedSignal(_labelPath);
  std::cout << "LabelPathAttr: Attribute removed" << std::endl;
}

Standard_Boolean LabelPathAttr::AfterUndo(
  const Handle(TDF_AttributeDelta)& anAttDelta,
  const Standard_Boolean forceIt)
{
  if (!anAttDelta.IsNull()) {
    if (anAttDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnRemoval))) {
      _shapeAddedSignal(_labelPath);
      std::cout << "LabelPathAttr: After undo → shape added" << std::endl;
    }
  }
  return TDF_Attribute::AfterUndo(anAttDelta, forceIt);
}