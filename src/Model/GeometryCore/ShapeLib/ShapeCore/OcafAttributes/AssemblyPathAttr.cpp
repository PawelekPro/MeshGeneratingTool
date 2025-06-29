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

#include "AssemblyPathAttr.hpp"

IMPLEMENT_STANDARD_RTTIEXT(AssemblyPathAttr, LabelPathAttr)

static const Standard_GUID GUID_ShapeIdAttr(
  "83f512d4-d099-4ca0-9146-7c4c8875e94c"
);

AssemblyPathAttr::AssemblyPathAttr()
: LabelPathAttr() {}

AssemblyPathAttr::AssemblyPathAttr(const std::vector<int>& labelPath)
: LabelPathAttr(labelPath) {}

const Standard_GUID& AssemblyPathAttr::GetID() {
  return GUID_ShapeIdAttr;
}

const Standard_GUID& AssemblyPathAttr::ID() const {
  return GetID();
}