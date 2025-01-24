/*
 * Copyright (C) 2024 Paweł Gilewicz
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

#include "QIVtkUtils.hpp"

vtkStandardNewMacro(QIVtkLookupTable);

//----------------------------------------------------------------------------
QIVtkLookupTable::QIVtkLookupTable() {
	AppDefaultColors appColors;

	double aRange[2];
	aRange[0] = QIVtkEntityType::ET_IsoLine;
	aRange[1] = QIVtkEntityType::ET_SeamEdge;

	this->Allocate(9);
	this->SetNumberOfTableValues(9);
	this->SetTableRange(aRange);
	this->SetValueRange(0, 1);

	// Use default colors stored in settings
	const AppDefaultColors::GeomColorsArray colorsArray = appColors.getGeometryEntitiesColorArray();
	this->setColorsTable(colorsArray);
}

//----------------------------------------------------------------------------
QIVtkLookupTable::~QIVtkLookupTable() { }

//----------------------------------------------------------------------------
vtkLookupTable* QIVtkLookupTable::GetAsBaseClass() {
	return this;
}

//----------------------------------------------------------------------------
void QIVtkLookupTable::setColorsTable(
	AppDefaultColors::GeomColorsArray colorsArray) {
	// Note: The order of elements in colorsArray is important

	int index = 0;
	for (const QColor& color : colorsArray) {
		this->SetTableValue(
			index, color.redF(), color.greenF(), color.blueF(), color.alphaF());
		++index;
	}
}