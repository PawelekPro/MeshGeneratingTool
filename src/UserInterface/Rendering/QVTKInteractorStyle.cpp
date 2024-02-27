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

vtkStandardNewMacro(Interactor::QVTKInteractorStyle);

// Interactor::QVTKInteractorStyle* Interactor::QVTKInteractorStyle::New() {
// 	return new Interactor::QVTKInteractorStyle(renWin);
// }

Interactor::QVTKInteractorStyle::QVTKInteractorStyle() = default;

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

	this->Superclass::OnRightButtonDown();
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
	this->Superclass::OnLeftButtonDown();
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

void Interactor::QVTKInteractorStyle::OnMouseMove() {
	if (this->hoveredActor) {
		this->hoveredActor->GetProperty()->DeepCopy(this->LastHoveredProperty);
	}

	int* clickPos = this->GetInteractor()->GetEventPosition();
	vtkNew<vtkCellPicker> picker;
	picker->SetTolerance(0.001);
	picker->Pick(clickPos[0], clickPos[1], 0, this->_qvtkRenderWindow->getRenderer());

	this->hoveredActor = picker->GetActor();

	// Sprawdź, czy coś zostało wybrane
	if (this->hoveredActor) {
		this->LastHoveredProperty->DeepCopy(this->hoveredActor->GetProperty());
		this->hoveredActor->GetProperty()->SetLineWidth(10);
		this->hoveredActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
	}
	// this->GetInteractor()->Render();
	this->Superclass::OnMouseMove();
}

void Interactor::QVTKInteractorStyle::Activate(Rendering::QVTKRenderWindow* qvtkRenderWindow) {
	_qvtkRenderWindow = qvtkRenderWindow;
	_contextMenu = nullptr;
	_customAction = nullptr;
	LastPickedActor = NULL;
	LastPickedProperty = vtkProperty::New();

	hoveredActor = NULL;
	LastHoveredProperty = vtkProperty::New();
}