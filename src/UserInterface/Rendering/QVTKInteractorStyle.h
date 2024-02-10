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

#ifndef QVTKINTERACTORSTYLE_H
#define QVTKINTERACTORSTYLE_H

#include "QVTKRenderWindow.h"

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>

#include <QAction>
#include <QFont>
#include <QMenu>

namespace Interactor {

/**
 * @brief  The QVTKInteractorStyle provides class for define custom interactions with render window.
 *
 */
class QVTKInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
	/**
	 * @brief  Return new instance of class.
	 *
	 * @param  {Rendering::QVTKRenderWindow*} renWin : Render window for which the interactor will be set.
	 * @return {QVTKInteractorStyle*}                : Instance of QVTKInteractorStyle class.
	 */
	static QVTKInteractorStyle* New(Rendering::QVTKRenderWindow* renWin);

	/**
	 * @brief Handle the right mouse button press event.
	 *
	 */
	virtual void OnRightButtonDown() override;

	virtual void OnLeftButtonDown() override;

	/**
	 * @brief  Get rendering window instance.
	 *
	 * @return {Rendering::QVTKRenderWindow*}  : Rendering window instance.
	 */
	Rendering::QVTKRenderWindow* getRenderWindow();

protected:
	QVTKInteractorStyle(Rendering::QVTKRenderWindow* qvtkRenderWindow);
	virtual ~QVTKInteractorStyle();

private:
	/**
	 * @brief Create context menu on position where mouse right button has been pressed.
	 *
	 */
	void createContextMenu();

	Rendering::QVTKRenderWindow* _qvtkRenderWindow;
	QMenu* _contextMenu;
	QAction* _customAction;

	vtkActor* LastPickedActor;
	vtkProperty* LastPickedProperty;
};
};

#endif