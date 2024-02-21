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

#include "QVTKNavigationWidget.h"

vtkStandardNewMacro(Navigation::QVTKAxesActor);

// Navigation::QVTKAxesActor* Navigation::QVTKAxesActor::New() {
// 	return new Navigation::QVTKAxesActor();
// }

void Navigation::QVTKNavigationWidget::drawNavigationWidget() {
}

void Navigation::QVTKNavigationWidget::drawCoordinateSystem() {
	double startPoint[3];
	double endPoint[3];

	startPoint[0] = 0;
	startPoint[1] = 0;
	startPoint[2] = 0;

	endPoint[0] = 0.05;
	endPoint[1] = 0;
	endPoint[2] = 0;

	// Compute a basis
	double normalizedX[3];
	double normalizedY[3];
	double normalizedZ[3];

	// Create an arrow.
	vtkNew<vtkArrowSource> arrowSource;
	arrowSource->SetTipResolution(100);
	arrowSource->SetShaftResolution(50);

	// The X axis is a vector from start to end
	vtkMath::Subtract(endPoint, startPoint, normalizedX);
	double length = vtkMath::Norm(normalizedX);
	vtkMath::Normalize(normalizedX);

	double arbitrary[3];
	arbitrary[0] = 1;
	arbitrary[1] = 2;
	arbitrary[2] = 3;
	vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
	vtkMath::Normalize(normalizedZ);

	vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
	vtkNew<vtkMatrix4x4> matrix;

	// Create the direction cosine matrix
	matrix->Identity();
	for (auto i = 0; i < 3; i++) {
		matrix->SetElement(i, 0, normalizedX[i]);
		matrix->SetElement(i, 1, normalizedY[i]);
		matrix->SetElement(i, 2, normalizedZ[i]);
	}

	// Apply the transforms
	vtkNew<vtkTransform> transform;
	transform->Translate(startPoint);
	transform->Concatenate(matrix);
	transform->Scale(length, length, length);

	// Transform the polydata
	vtkNew<vtkTransformPolyDataFilter> transformPD;
	transformPD->SetTransform(transform);
	transformPD->SetInputConnection(arrowSource->GetOutputPort());

	// Create a mapper and actor for the arrow
	vtkNew<vtkPolyDataMapper> mapper;
	vtkNew<vtkActor> actor;

	mapper->SetInputConnection(transformPD->GetOutputPort());
	actor->SetMapper(mapper);

	_viewer->addActor(actor);
}
