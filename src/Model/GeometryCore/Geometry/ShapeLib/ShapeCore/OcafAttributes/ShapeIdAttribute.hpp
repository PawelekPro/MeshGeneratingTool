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

#ifndef SHAPEIDATTRIBUTE_HPP
#define SHAPEIDATTRIBUTE_HPP

#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <Standard_OStream.hxx>

class ShapeIdAttribute;

// Define a handle to our attribute
DEFINE_STANDARD_HANDLE(ShapeIdAttribute, TDF_Attribute)

class ShapeIdAttribute : public TDF_Attribute {
public:
    
DEFINE_STANDARD_RTTIEXT(ShapeIdAttribute, TDF_Attribute)

    static const Standard_GUID& GetID();
    const Standard_GUID& ID() const override;

    Handle(TDF_Attribute) NewEmpty() const override;

    void Restore(const Handle(TDF_Attribute)& from) override;

    void Paste(
        const Handle(TDF_Attribute)&  into,
        const Handle(TDF_RelocationTable)&  table
    ) const override;

    Standard_OStream& Dump(Standard_OStream& os) const override;

    ShapeIdAttribute();
    ShapeIdAttribute(Standard_Integer labelTag,
                    Standard_Integer parentLabelTag);

    void Set(Standard_Integer labelTag,
            Standard_Integer parentLabelTag);

    void Get(Standard_Integer& labelTag,
            Standard_Integer& parentLabelTag) const;

    void AfterAddition() override;
    
    void BeforeRemoval() override;

private:
  Standard_Integer _labelTag;
  Standard_Integer _parentLabelTag;
};

#endif