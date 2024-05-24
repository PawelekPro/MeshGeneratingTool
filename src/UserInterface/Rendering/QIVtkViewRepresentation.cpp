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

#include "QIVtkViewRepresentation.h"

QIVtkViewRepresentation::QIVtkViewRepresentation()
	: _colorTable(vtkSmartPointer<vtkLookupTable>::New()) {
	double aRange[2];
	aRange[0] = QIVtkEntityType::ET_IsoLine;
	aRange[1] = QIVtkEntityType::ET_SeamEdge;

	_colorTable->Allocate(9);
	_colorTable->SetNumberOfTableValues(9);
	_colorTable->SetTableRange(aRange);
	_colorTable->SetValueRange(0, 1);
}

QIVtkViewRepresentation::~QIVtkViewRepresentation() { }

void convertQColorToRGBA(const QColor& color, double rgba[4]) {
	rgba[0] = color.red() / 255.0;
	rgba[1] = color.green() / 255.0;
	rgba[2] = color.blue() / 255.0;
	rgba[3] = color.alpha() / 255.0;
}

void QIVtkViewRepresentation::setColorsTable(QList<QColor> colorList) {
	// Note: The order of elements in colorList is important

	int index = 0;
	double rgba[4];
	for (const auto& color : colorList) {
		convertQColorToRGBA(color, rgba);
		_colorTable->SetTableValue(index, rgba);
	}
	// aColorTable->SetTableValue(0, 0, 0, 0); // Undefined
	// aColorTable->SetTableValue(1, 0.5, 0.5, 0.5); // gray for IsoLine
	// aColorTable->SetTableValue(2, 1, 0, 0); // red for Free vertex
	// aColorTable->SetTableValue(3, 0, 0, 0); // Shared vertex
	// aColorTable->SetTableValue(4, 1, 0, 0); // red for Free edge
	// aColorTable->SetTableValue(5, 0, 1, 0); // green for Boundary edge (related to a single face)
	// aColorTable->SetTableValue(6, 0, 0, 0); // Shared edge (related to several faces)
	// aColorTable->SetTableValue(7, 1, 1, 0); // yellow for Wireframe face
	// aColorTable->SetTableValue(8, 0.35, 0.35, 0.35); // Shaded face
}