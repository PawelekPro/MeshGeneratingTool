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
static void ClearHighlightAndSelection(const Handle(QIVtkSelectionPipeline) & thePipeline,
	const Standard_Boolean doHighlighting,
	const Standard_Boolean doSelection) {
	if (!doHighlighting && !doSelection) {
		return;
	}

	if (doHighlighting) {
		thePipeline->ClearHighlightFilters();
	}

	if (doSelection) {
		thePipeline->ClearSelectionFilters();
	}
}

//----------------------------------------------------------------------------
vtkStandardNewMacro(Interactor::QVTKInteractorStyle);

//----------------------------------------------------------------------------
Interactor::QVTKInteractorStyle::QVTKInteractorStyle()
	: vtkInteractorStyleTrackballCamera() { }

//----------------------------------------------------------------------------
Interactor::QVTKInteractorStyle::~QVTKInteractorStyle() {
	if (_contextMenu)
		delete _contextMenu;

	// lastPickedProperty->Delete();
	// lastHoveredProperty->Delete();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::setRenderer(
	const vtkSmartPointer<vtkRenderer>& theRenderer) {
	m_renderer = theRenderer;
}

//----------------------------------------------------------------------------
Rendering::QVTKRenderWindow*
Interactor::QVTKInteractorStyle::getRenderWindow() {
	return this->_qvtkRenderWindow;
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkRenderer>
Interactor::QVTKInteractorStyle::getRenderer() const {
	return m_renderer;
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::setPicker(
	const vtkSmartPointer<IVtkTools_ShapePicker>& thePicker) {
	m_picker = thePicker;
}

//----------------------------------------------------------------------------
vtkSmartPointer<IVtkTools_ShapePicker>
Interactor::QVTKInteractorStyle::getPicker() const {
	return m_picker;
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::setPipeline(
	const Handle(QIVtkSelectionPipeline) pipeline) {
	m_pipeline = pipeline;
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::createContextMenu() {
	if (!_contextMenu) {
		_contextMenu = new QMenu;

		// Set font size
		QFont font;
		font.setPointSize(10);
		_contextMenu->setFont(font);

		_fitViewAction = new QAction("Fit view", _contextMenu);
		_faceSizingAction = new QAction("Add face sizing", _contextMenu);
		_edgeSizingAction = new QAction("Add edge sizing", _contextMenu);

		QObject::connect(_fitViewAction, &QAction::triggered, [this]() {
			this->_qvtkRenderWindow->fitView();
		});
		_contextMenu->addAction(_fitViewAction);
		_contextMenu->addAction(_faceSizingAction);
		_contextMenu->addAction(_edgeSizingAction);
	}
	_edgeSizingAction->setDisabled(true);
	_faceSizingAction->setDisabled(true);
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnRightButtonDown() {
	this->createContextMenu();
	_contextMenu->exec(QCursor::pos());

	// this->Superclass::OnRightButtonDown();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnLeftButtonDown() {

	if (this->Interactor->GetShiftKey()) {
		std::cout << "Detected Shift key event." << std::endl;
	}

	this->Superclass::OnLeftButtonDown();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnMouseMove() {
	this->Superclass::OnMouseMove();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnKeyPress() {
	this->Superclass::OnKeyPress();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::OnKeyRelease() {
	this->Superclass::OnKeyRelease();
}

//----------------------------------------------------------------------------
void Interactor::QVTKInteractorStyle::Initialize(Rendering::QVTKRenderWindow* qvtkRenderWindow) {
	_qvtkRenderWindow = qvtkRenderWindow;
	_contextMenu = nullptr;
	_fitViewAction = nullptr;
}