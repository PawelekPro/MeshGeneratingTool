/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 *
 * Created by Paweł Gilewicz on 01/02/2024.
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
	, _qvtkRenderWindow(qvtkRenderWindow) { }

Interactor::QVTKInteractorStyle::~QVTKInteractorStyle() {
	if (_contextMenu)
		delete _contextMenu;
}

Rendering::QVTKRenderWindow* Interactor::QVTKInteractorStyle::getRenderWindow() {
	return this->_qvtkRenderWindow;
}

void Interactor::QVTKInteractorStyle::OnRightButtonDown() {
	this->createContextMenu();
	_contextMenu->exec(QCursor::pos());

	vtkInteractorStyle::OnRightButtonDown();
}

void Interactor::QVTKInteractorStyle::createContextMenu() {
	if (!_contextMenu) {
		_contextMenu = new QMenu;
		_customAction = new QAction("Fit view", _contextMenu);

		QObject::connect(_customAction, &QAction::triggered, [this]() {
			this->_qvtkRenderWindow->fitView();
		});

		// Add the custom QAction to the context menu
		_contextMenu->addAction(_customAction);
	}
}