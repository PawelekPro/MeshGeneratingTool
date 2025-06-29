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

#include "ShapePathAttr.hpp"

IMPLEMENT_STANDARD_RTTIEXT(ShapePathAttr, LabelPathAttr)

static const Standard_GUID GUID_ShapeIdAttr(
  "f5db02f3-a2f5-4921-ba3e-86cbe73420f4"
);

ShapePathAttr::ShapePathAttr()
: LabelPathAttr() {}

ShapePathAttr::ShapePathAttr(const std::vector<int>& labelPath)
: LabelPathAttr(labelPath) {}

const Standard_GUID& ShapePathAttr::GetID() {
  return GUID_ShapeIdAttr;
}

const Standard_GUID& ShapePathAttr::ID() const {
  return GetID();
}

