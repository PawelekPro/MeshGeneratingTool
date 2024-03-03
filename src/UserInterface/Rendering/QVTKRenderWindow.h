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

#ifndef QVTKRENDERWINDOW_H
#define QVTKRENDERWINDOW_H

#include "GeometryFunctions.h"
#include "QVTKToolBar.h"

#include <QVTKOpenGLNativeWidget.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <vtkInteractorStyle.h>

#include <vtkButtonWidget.h>
#include <vtkTexturedButtonRepresentation2D.h>

namespace Rendering {

/**
 * @brief  The QVTKRenderWindow provides class for rendering VTK objects inside Qt application.
 *
 */
class QVTKRenderWindow {
public:
	QVTKRenderWindow(QWidget* widget);
	~QVTKRenderWindow();

	// The VTK renderer (add VTK actors to it to build the scene)
	vtkSmartPointer<vtkRenderer> _renderer;

	vtkSmartPointer<vtkRenderer> _edgeRenderer;

	vtkSmartPointer<vtkRenderer> activeLayerRenderer;

	/**
	 * @brief Generate global coordinate system.
	 *
	 */
	void generateCoordinateSystemAxes();

	/**
	 * @brief  Add vtkActors to the current renderer and display them.
	 *
	 * @param  {Importing::ActorsMap} actorsMap : Container of vtkActors.
	 */
	void addActors(const Geometry::ActorsMap& actorsMap);

	/**
	 * @brief  Add vtkActors to the current renderer and display them.
	 *
	 * @param  {Importing::ActorsMap} actorsMap : Container of vtkActors.
	 */
	void addEdgesActors(const Geometry::ActorsMap& actorsMap);

	/**
	 * @brief  Add actor to the renderer.
	 *
	 * @param  {vtkActor*} actor : vtkActor object.
	 */
	void addActor(vtkActor* actor);

	/**
	 * @brief  Set interator style to customize interaction.
	 *
	 * @param  {vtkInteractorStyle*} interactorStyle : Custom interaction style.
	 */
	void setInteractorStyle(vtkInteractorStyle* interactorStyle);

	/**
	 * @brief Adjust the displayed objects to the size of the rendering window.
	 *
	 */
	void fitView();

	vtkRenderer* getRenderer() { return this->activeLayerRenderer; }

	vtkRenderWindowInteractor* getInteractor() { return this->_rendererWindow->GetInteractor(); }

	vtkOrientationMarkerWidget* getOrientationMarkerWidget() { return this->_vtkAxesWidget; }

	void enableCameraOrientationWidget() {
		_camOrientManipulator->On();
		_toolBar->On();
		_toolBar->createDefaultGeometry();
	}

	void setActiveLayerRenderer(int layer) {
		if (layer == 0) {
			this->activeLayerRenderer = _renderer;
		} else {
			this->activeLayerRenderer = _edgeRenderer;
		}
	}

	void addButton();

	void enableButton() {
		// this->buttonWidget->On();
	}

private:
	QWidget* _widget;

	// The Qt widget containing a VTK viewport
	QVTKOpenGLNativeWidget* _vtkWidget;

	// The VTK renderer window
	vtkSmartPointer<vtkRenderWindow> _rendererWindow;

	// The VTK render window interactor
	vtkSmartPointer<vtkRenderWindowInteractor> _interactor;

	// Widget for displaying global coordinate system
	vtkSmartPointer<vtkOrientationMarkerWidget> _vtkAxesWidget;

	vtkNew<vtkCameraOrientationWidget> _camOrientManipulator;

	vtkNew<ToolBar::QVTKToolBar> _toolBar;

	// vtkSmartPointer<vtkTexturedButtonRepresentation2D> buttonRepresentation;
	// vtkSmartPointer<vtkButtonWidget> buttonWidget;
};
}; // namespace Rendering

#endif