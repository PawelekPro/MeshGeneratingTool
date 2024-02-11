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

#include "QVTKInteractorStyle.h"

#include <QAction>

Interactor::QVTKInteractorStyle* Interactor::QVTKInteractorStyle::New(Rendering::QVTKRenderWindow* renWin) {
	return new Interactor::QVTKInteractorStyle(renWin);
}

Interactor::QVTKInteractorStyle::QVTKInteractorStyle(Rendering::QVTKRenderWindow* qvtkRenderWindow)
	: _contextMenu(nullptr)
	, _customAction(nullptr)
	, _qvtkRenderWindow(qvtkRenderWindow) {

	LastPickedActor = NULL;
	LastPickedProperty = vtkProperty::New();
}

Interactor::QVTKInteractorStyle::~QVTKInteractorStyle() {
	if (_contextMenu)
		delete _contextMenu;

	LastPickedProperty->Delete();
}

Rendering::QVTKRenderWindow* Interactor::QVTKInteractorStyle::getRenderWindow() {
	return this->_qvtkRenderWindow;
}

void Interactor::QVTKInteractorStyle::OnRightButtonDown() {
	this->createContextMenu();
	_contextMenu->exec(QCursor::pos());

	vtkInteractorStyle::OnRightButtonDown();
}

void Interactor::QVTKInteractorStyle::OnLeftButtonDown() {
	vtkNew<vtkNamedColors> colors;
	int* clickPos = this->GetInteractor()->GetEventPosition();

	// Pick from this location
	vtkNew<vtkPropPicker> picker;
	picker->Pick(clickPos[0], clickPos[1], 0, this->_qvtkRenderWindow->getRenderer());

	// If picked something before, reset its property.
	if (this->LastPickedActor) {
		this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
	}
	this->LastPickedActor = picker->GetActor();
	if (this->LastPickedActor) {
		// Save the property of the pciked actor so that it can be restored next time.
		this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());

		// Highlight the picked actor
		this->LastPickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
		this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
		this->LastPickedActor->GetProperty()->SetSpecular(0.0);
		// this->LastPickedActor->GetProperty()->EdgeVisibilityOn();
	}

	// Forward events
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void Interactor::QVTKInteractorStyle::createContextMenu() {
	if (!_contextMenu) {
		_contextMenu = new QMenu;

		// Set font size
		QFont font;
		font.setPointSize(10);
		_contextMenu->setFont(font);

		_customAction = new QAction("Fit view", _contextMenu);

		QObject::connect(_customAction, &QAction::triggered, [this]() {
			this->_qvtkRenderWindow->fitView();
		});

		// Add the custom QAction to the context menu
		_contextMenu->addAction(_customAction);
	}
}