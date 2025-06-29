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

#ifndef LabelPathAttr_HPP
#define LabelPathAttr_HPP

#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <Standard_OStream.hxx>

#include <boost/signals2.hpp>

class LabelPathAttr;
DEFINE_STANDARD_HANDLE(LabelPathAttr, TDF_Attribute)

class LabelPathAttr : public TDF_Attribute {

public:
	DEFINE_STANDARD_RTTIEXT(LabelPathAttr, TDF_Attribute)
	



	static const Standard_GUID& GetID();
	const Standard_GUID& ID() const override;

	Handle(TDF_Attribute) NewEmpty() const override;

	void Restore(const Handle(TDF_Attribute)& from) override;

	void Paste(
		const Handle(TDF_Attribute)&  into,
		const Handle(TDF_RelocationTable)&  table
	) const override;

	Standard_OStream& Dump(Standard_OStream& os) const override;

	LabelPathAttr();
	LabelPathAttr(const std::vector<int>& labelPath);

	void Set(const std::vector<int>& labelPath);
	void Get(std::vector<int>& labelPath) const;

	void AfterAddition() override;
	void BeforeRemoval() override;

	Standard_Boolean AfterUndo (
		const Handle(TDF_AttributeDelta)& anAttDelta, 
		const Standard_Boolean forceIt = Standard_False
	) override;

	
	std::vector<int> labelPath() const {return _labelPath;}    
	
	using LabelPathSignal = boost::signals2::signal<
		void(std::vector<int> labelPath)
	>;
	
	LabelPathSignal& shapeAddedSignal() { return _shapeAddedSignal; }
    LabelPathSignal& shapeRemovedSignal() { return _shapeRemovedSignal; }
	
	protected:
	
	LabelPathSignal _shapeAddedSignal;
	LabelPathSignal _shapeRemovedSignal;
	std::vector<int> _labelPath;
};

#endif