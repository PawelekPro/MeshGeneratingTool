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

//----------------------------------------------------------------------------
vtkStandardNewMacro(Interactor::QVTKInteractorStyle);

// Interactor::QVTKInteractorStyle* Interactor::QVTKInteractorStyle::New() {
// 	return new Interactor::QVTKInteractorStyle(renWin);
// }

//----------------------------------------------------------------------------
Interactor::QVTKInteractorStyle::QVTKInteractorStyle() = default;

//----------------------------------------------------------------------------
Interactor::QVTKInteractorStyle::~QVTKInteractorStyle() {
	if (_contextMenu)
		delete _contextMenu;

	lastPickedProperty->Delete();
	lastHoveredProperty->Delete();
}

//----------------------------------------------------------------------------
Rendering::QVTKRenderWindow* Interactor::QVTKInteractorStyle::getRenderWindow() {
	return this->_qvtkRenderWindow;
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnRightButtonDown() {
	this->createContextMenu();
	_contextMenu->exec(QCursor::pos());

	this->Superclass::OnRightButtonDown();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnLeftButtonDown() {
	vtkNew<vtkNamedColors> colors;
	int* clickPos = this->GetInteractor()->GetEventPosition();

	// Pick from this location
	vtkNew<vtkPropPicker> picker;
	picker->Pick(clickPos[0], clickPos[1], 0, this->_qvtkRenderWindow->getRenderer());

	// If picked something before, reset its property.
	if (this->lastPickedActor) {
		this->lastPickedActor->GetProperty()->DeepCopy(this->lastPickedProperty);
	}
	this->lastPickedActor = picker->GetActor();
	if (this->lastPickedActor) {
		// Save the property of the pciked actor so that it can be restored next time.
		this->lastPickedProperty->DeepCopy(this->lastPickedActor->GetProperty());

		// Highlight the picked actor
		this->lastPickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
		this->lastPickedActor->GetProperty()->SetDiffuse(1.0);
		this->lastPickedActor->GetProperty()->SetSpecular(0.0);
		// this->LastPickedActor->GetProperty()->EdgeVisibilityOn();
	}

	// Forward events
	this->Superclass::OnLeftButtonDown();
}

//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnMouseMove() {
	if (this->lastHoveredActor) {
		this->lastHoveredActor->GetProperty()->DeepCopy(this->lastHoveredProperty);
	}

	this->lastHoveredActor = NULL;

	int* clickPos = this->GetInteractor()->GetEventPosition();
	vtkNew<vtkCellPicker> picker;
	picker->SetTolerance(0.001);
	picker->Pick(clickPos[0], clickPos[1], 0, this->_qvtkRenderWindow->getRenderer());

	this->lastHoveredActor = picker->GetActor();

	// Check if something was picked
	if (this->lastHoveredActor) {
		this->lastHoveredProperty->DeepCopy(this->lastHoveredActor->GetProperty());
		this->lastHoveredActor->GetProperty()->SetLineWidth(5);
		this->lastHoveredActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
	}
	// this->GetInteractor()->Render();
	this->Superclass::OnMouseMove();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::Initialize(Rendering::QVTKRenderWindow* qvtkRenderWindow) {
	_qvtkRenderWindow = qvtkRenderWindow;
	_contextMenu = nullptr;
	_customAction = nullptr;
	lastPickedActor = NULL;
	lastPickedProperty = vtkProperty::New();

	lastHoveredActor = NULL;
	lastHoveredProperty = vtkProperty::New();
}