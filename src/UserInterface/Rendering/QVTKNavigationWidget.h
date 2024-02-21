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

#include "QVTKRenderWindow.h"
#include <vtkArrowSource.h>
#include <vtkAxesActor.h>
#include <vtkCellArray.h>
#include <vtkLineSource.h>
#include <vtkMath.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropAssembly.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

namespace Navigation {

class QVTKAxesActor : public vtkPropAssembly {
public:
	static QVTKAxesActor* New();
	vtkTypeMacro(QVTKAxesActor, vtkPropAssembly);

protected:
	QVTKAxesActor() { this->drawCoordinateSystem(); }
	~QVTKAxesActor() { }

	vtkNew<vtkAxesActor> MakeAxesActor() {
		vtkNew<vtkAxesActor> axes;
		return axes;
	}

	vtkNew<vtkActor> MakeLineActor() {
		// vtkNew<vtkLineSource> line;
		// line->SetPoint1(0.0, 0.5, 0.0);
		// line->SetPoint2(0.5, 0.5, 0.0);
		// vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		// lineMapper->SetInputConnection(line->GetOutputPort());
		// vtkNew<vtkActor> lineActor;
		// lineActor->SetMapper(lineMapper);
		// return lineActor;

		vtkNew<vtkPlaneSource> planeSource;
		// planeSource->SetCenter(1.0, 0.0, 0.0);
		// planeSource->SetNormal(1.0, 0.0, 0.0);
		planeSource->SetOrigin(0.0, 0.0, 0.0);
		planeSource->SetPoint1(0.5, 0, 0.0);
		planeSource->SetPoint2(0, 0.5, 0.0);

		planeSource->Update();
		vtkPolyData* plane = planeSource->GetOutput();

		// Create a mapper and actor
		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputData(plane);

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);
		return actor;
	}

	void drawCoordinateSystem() {
		auto axes = MakeAxesActor();
		auto line = MakeLineActor();
		this->AddPart(axes);
		this->AddPart(line);
	}
};

class QVTKNavigationWidget : public vtkOrientationMarkerWidget {
public:
	// vtkTypeMacro(QVTKNavigationWidget, vtkOrientationMarkerWidget);
	QVTKNavigationWidget(Rendering::QVTKRenderWindow* qvtkRenderWindow)
		: _viewer(qvtkRenderWindow) {
		vtkSmartPointer<QVTKAxesActor> axes = vtkSmartPointer<QVTKAxesActor>::New();
		this->SetOrientationMarker(axes);
	}
	~QVTKNavigationWidget() { }

	static vtkSmartPointer<QVTKNavigationWidget> New(Rendering::QVTKRenderWindow* renWin) {
		return vtkSmartPointer<QVTKNavigationWidget>(new QVTKNavigationWidget(renWin));
	}

	void drawCoordinateSystem();

	void drawNavigationWidget();
	void drawNavigationWidget(bool pickMode, float opacity);

private:
	Rendering::QVTKRenderWindow* _viewer;
};

}