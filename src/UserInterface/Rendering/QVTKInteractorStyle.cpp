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

	// lastPickedProperty->Delete();
	// lastHoveredProperty->Delete();
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

	int* clickPos = this->GetInteractor()->GetEventPosition();
	LMBPicker->Pick(clickPos[0], clickPos[1], 0, this->_qvtkRenderWindow->getActiveRenderer());
	LMBActor = this->LMBPicker->GetActor();
    if (LMBActor != prevLMBActor) {
        if (LMBActor) {
            if (prevLMBActor) {
                prevLMBActor->SetProperty(prevLMBProperty);
            }
            prevLMBProperty->DeepCopy(prevHoveredProperty);
            LMBActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
            LMBActor->GetProperty()->SetLineWidth(5);
        } else {
            if (prevLMBActor) {
                prevLMBActor->SetProperty(prevLMBProperty);
                prevLMBProperty = nullptr;
            }
        }
    }
	prevLMBActor = LMBActor;
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
    int* clickPos = this->GetInteractor()->GetEventPosition();
    hoverPicker->Pick(clickPos[0], clickPos[1], 0, this->_qvtkRenderWindow->getActiveRenderer());
    hoveredActor = this->hoverPicker->GetActor();
    if (hoveredActor != prevHoveredActor) {
        if (hoveredActor) {
            if (prevHoveredActor) {
				if(prevHoveredActor != prevLMBActor){
                	prevHoveredActor->SetProperty(prevHoveredProperty);
				}
            }
			if (hoveredActor!= prevLMBActor){
				hoveredActor->GetProperty()->DeepCopy(prevHoveredProperty);
				hoveredActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
				hoveredActor->GetProperty()->SetLineWidth(5);
			}
        } else {
            if (prevHoveredActor) {
				if(prevHoveredActor != prevLMBActor){
					prevHoveredActor->SetProperty(prevHoveredProperty);
				}
				prevHoveredProperty = nullptr;
            }
        }
    }
    this->prevHoveredActor = hoveredActor ? hoveredActor : nullptr;
    this->Superclass::OnMouseMove();
}

void Interactor::QVTKInteractorStyle::OnKeyPress() {
    std::string key = this->GetInteractor()->GetKeySym();
    if (key == "Shift_L" || key == "Shift_R") {
        this->shiftPressed = true;
    }
    this->Superclass::OnKeyPress();
}

void Interactor::QVTKInteractorStyle::OnKeyRelease() {
    std::string key = this->GetInteractor()->GetKeySym();
    if (key == "Shift_L" || key == "Shift_R") {
        this->shiftPressed = false;
    }
    this->Superclass::OnKeyRelease();
}


//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::Initialize(Rendering::QVTKRenderWindow* qvtkRenderWindow) {
	_qvtkRenderWindow = qvtkRenderWindow;
	_contextMenu = nullptr;
	_customAction = nullptr;


	LMBActor = nullptr;
	prevLMBActor = nullptr;
	prevLMBProperty  = vtkProperty::New();
	LMBPicker = vtkPropPicker::New();;


	hoveredActor = nullptr;
	prevHoveredActor = nullptr;
	prevHoveredProperty = vtkProperty::New();
	hoverPicker = vtkPropPicker::New();
}