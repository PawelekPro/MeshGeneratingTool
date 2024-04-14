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

#include "Model.h"
#include "Mesh.h"
#include "Geometry.h"

#include <algorithm>
#include <numeric>
#include <array>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyle.h>
#include <vtkLogger.h>
#include <vtkLogoRepresentation.h>
#include <vtkLogoWidget.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPNGReader.h>
#include <vtkQImageToImageSource.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCaptionActor2D.h>
#include <vtkNamedColors.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>

#include <QImage>
#include <QPixmap>
#include <QPointer>

namespace Rendering {
enum class Renderers {
	Main,
	Faces,
	Edges,
	Count // Count for definition of array size
};
enum class Layers {
	Bottom,
	Top,
	Count // Count for definition of array size
};
/**
 * @brief  The QVTKRenderWindow provides class for rendering VTK objects inside Qt application.
 *
 */
class QVTKRenderWindow {

public:
	QVTKRenderWindow(QWidget* widget);
	~QVTKRenderWindow();
	std::shared_ptr<Model> model;
	/**
	 * @brief Generate global coordinate system.
	 *
	 */
	void generateCoordinateSystemAxes();
	/**
	 * @brief  Set interator style to customize interaction.
	 *
	 * @param  {vtkInteractorStyle*} interactorStyle : Custom interaction style.
	 */
	void setInteractorStyle(vtkInteractorStyle* interactorStyle);

	/**
	 * @brief  Adjust the displayed objects to the size of the rendering window.
	 *
	 */
	void fitView();

	/**
	 * @brief  Reset render view to update currently displayed state.
	 *
	 */
	void RenderScene();



	/**
	 * @brief  Enable and start displaying the camera orientation widget.
	 *
	 */
	void enableCameraOrientationWidget();

	/**
	 * @brief  Enable and start displaying the water mark widget.
	 *
	 */
	void enableWaterMark();

	/**
	 * @brief  Set active renderer
	 *
	 * @param  {Renderers} layer : ID of renderer to be activated.
	 */
	void setActiveRenderer(Renderers rendererId);
	/**
	 * @brief  Get renderer representing currently active layer.
	 *
	 * @return {vtkRenderer*}  : vtkRenderer instance representing active layer
	 */
	vtkSmartPointer<vtkRenderer> getActiveRenderer();
	/**
	 * @brief  Initialize and setup water mark widget.
	 *
	 */
	void setWaterMark();


	/**
	 * @brief  Clear all geometry actors from faces, edges and parts layers and
	 * reload them from Geometry object
	 *
	 */
	void updateGeometryActors(const GeometryCore::Geometry& geometry);

protected:
	/**
	 * @brief  Construct and initialize renderes at each layer.
	 *
	 */
	void initializeRenderers();

	// Definition of array of pointers to VTK renderers
	std::array<vtkSmartPointer<vtkRenderer>, static_cast<size_t>(Renderers::Count)> mRenderers;

	// Attribute for storing the currently active renderer (layer)
	vtkSmartPointer<vtkRenderer> activeRenderer;
	Renderers activeRendererId;

private:
	QPointer<QWidget> _widget;

	// The Qt widget containing a VTK viewport
	QVTKOpenGLNativeWidget* _vtkWidget;

	// The VTK renderer window
	vtkSmartPointer<vtkRenderWindow> _rendererWindow;

	// The VTK render window interactor
	vtkSmartPointer<vtkRenderWindowInteractor> _interactor;

	// Widget for displaying global coordinate system
	vtkSmartPointer<vtkOrientationMarkerWidget> _vtkAxesWidget;

	// Navigation widget
	vtkNew<vtkCameraOrientationWidget> _camOrientManipulator;

	// Water mark
	vtkSmartPointer<vtkLogoWidget> _logoWidget;
};
};

#endif