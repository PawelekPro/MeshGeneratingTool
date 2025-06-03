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

#include <TDF_RelocationTable.hxx>
#include <TCollection_AsciiString.hxx>
#include "ShapeKeyAttr.hpp"

IMPLEMENT_STANDARD_RTTIEXT(ShapeKeyAttr, Standard_Transient)

ShapeKeyAttr::ShapeKeyAttr()
: _labelTag(0), _parentLabelTag(0) {}

ShapeKeyAttr::ShapeKeyAttr(
  Standard_Integer aLabelTag,
  Standard_Integer aParentLabelTag
)
: _labelTag(aLabelTag), _parentLabelTag(aParentLabelTag) {}


static const Standard_GUID GUID_ShapeIdAttr(
  "2ae4362f-ce33-4af9-909e-f009ad8516f3"
);

const Standard_GUID& ShapeKeyAttr::GetID() {
  return GUID_ShapeIdAttr;
}

const Standard_GUID& ShapeKeyAttr::ID() const {
  return GetID();
}

Handle(TDF_Attribute) ShapeKeyAttr::NewEmpty() const {
  return new ShapeKeyAttr();
}

void ShapeKeyAttr::Restore(const Handle(TDF_Attribute)& from) {
  const Handle(ShapeKeyAttr) src =
    Handle(ShapeKeyAttr)::DownCast(from);
  _labelTag = src->_labelTag;
  _parentLabelTag  = src->_parentLabelTag;
}

void ShapeKeyAttr::Paste(
  const Handle(TDF_Attribute)& into,
  const Handle(TDF_RelocationTable)&
) const {
  const Handle(ShapeKeyAttr) dst =
    Handle(ShapeKeyAttr)::DownCast(into);
  dst->_labelTag = _labelTag;
  dst->_parentLabelTag  = _parentLabelTag;
}

Standard_OStream& ShapeKeyAttr::Dump(Standard_OStream& os) const {
  os << "ShapeKeyAttr(LabelTag=" << _labelTag
     << ", parentLabelTag=" << _parentLabelTag << ")";
  return os;
}

void ShapeKeyAttr::Set(
  Standard_Integer aLabelTag,
  Standard_Integer aParentLabelTag
) {
  if (_labelTag != aLabelTag || _parentLabelTag != aParentLabelTag) {
    Backup();
    _labelTag = aLabelTag;
    _parentLabelTag  = aParentLabelTag;
  }
}

void ShapeKeyAttr::Get( 
	Standard_Integer& labelTag, 
	Standard_Integer& shapeId
) const {
  labelTag = _labelTag;
  shapeId  = _parentLabelTag;
}

#include <iostream>
void ShapeKeyAttr::AfterAddition(){
	_shapeAddedSignal(_labelTag, _parentLabelTag);
    std::cout << "Attribute added" << std::endl;
}

void ShapeKeyAttr::BeforeRemoval(){
	_shapeRemovedSignal(_labelTag, _parentLabelTag);
	std::cout << "Attribute removed" << std::endl;
}