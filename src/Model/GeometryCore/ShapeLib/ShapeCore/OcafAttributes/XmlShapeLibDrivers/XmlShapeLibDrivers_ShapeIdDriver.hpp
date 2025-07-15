/*
* Copyright (C) 2024 Krystian Fudali
*
* This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*s
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XmlShapeLibDrivers_ShapeIdDriver_HPP
#define XmlShapeLibDrivers_ShapeIdDriver_HPP

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <XmlMDF_ADriver.hxx>
#include <XmlMDF_ADriverTable.hxx>
#include <XmlObjMgt_RRelocationTable.hxx>
#include <XmlObjMgt_SRelocationTable.hxx>

class Message_Messenger;
class TDF_Attribute;
class XmlObjMgt_Persistent;
class XmlShapeLibDrivers_ShapeIdDriver;

DEFINE_STANDARD_HANDLE(XmlShapeLibDrivers_ShapeIdDriver, XmlMDF_ADriver);

class XmlShapeLibDrivers_ShapeIdDriver : public XmlMDF_ADriver
{
	public:
	DEFINE_STANDARD_RTTIEXT(XmlShapeLibDrivers_ShapeIdDriver, XmlMDF_ADriver)

	Standard_EXPORT virtual Handle(TDF_Attribute) NewEmpty() const override;

	Standard_EXPORT Handle(Standard_Type) SourceType() const override;
	
	Standard_EXPORT Standard_Boolean Paste(
		const XmlObjMgt_Persistent& theSource,
		const Handle(TDF_Attribute)& theTarget,
		XmlObjMgt_RRelocationTable& theRelocTable
	) const Standard_OVERRIDE;

	Standard_EXPORT void Paste(
		const Handle(TDF_Attribute)& theSource,
		XmlObjMgt_Persistent& theTarget,
		XmlObjMgt_SRelocationTable& theRelocTable
	) const Standard_OVERRIDE;

	Standard_EXPORT XmlShapeLibDrivers_ShapeIdDriver(
		const Handle(Message_Messenger)& theMsgDriver,
		Standard_CString                theName
	);
};
#endif 