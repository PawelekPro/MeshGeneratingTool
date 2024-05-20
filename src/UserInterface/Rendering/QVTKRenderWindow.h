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

#include "QVTKInteractorStyle.h"

#include "Geometry.h"
#include "Mesh.h"
#include "Model.h"

#include <algorithm>
#include <array>
#include <numeric>

#include <QVTKOpenGLNativeWidget.h>
#include <QVTKOpenGLWindow.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkCaptionActor2D.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyle.h>
#include <vtkLogger.h>
#include <vtkLogoRepresentation.h>
#include <vtkLogoWidget.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPNGReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQImageToImageSource.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTextProperty.h>

// VIS includes
#include <IVtkTools_ShapePicker.hxx>

#include <QImage>
#include <QPixmap>
#include <QPointer>

namespace Rendering {

/**
 * @brief  The QVTKRenderWindow provides class for rendering VTK objects inside Qt application.
 *
 */
class QVTKRenderWindow {

public:
	QVTKRenderWindow(QWidget* widget);
	~QVTKRenderWindow();

	/**
	 * @brief Generate global coordinate system.
	 *
	 */
	void generateCoordinateSystemAxes();

	/**
	 * @brief  Adjust the displayed objects to the size of the rendering window.
	 *
	 */
	void fitView();

	void addActor(vtkActor* actor);

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
	 * @brief  Initialize and setup water mark widget.
	 *
	 */
	void setWaterMark();


	/**
	 * @brief  Create pipeline based on TopoDS_Shape and add it to render window
	 *
	 */
	void addShapeToRenderer(const TopoDS_Shape& shape);

public:
	std::shared_ptr<Model> model;

private:
	TopoDS_Shape theShape;
	QIVtkSelectionPipeline* pipeline;

	QPointer<QWidget> _widget;

	// The Qt widget containing a VTK viewport
	QPointer<QVTKOpenGLNativeWidget> _vtkWidget;

	// The VTK renderer window
	vtkGenericOpenGLRenderWindow* _rendererWindow;

	// The VTK renderer
	vtkSmartPointer<vtkRenderer> _renderer;

	// The VTK render window interactor
	QVTKInteractor* _interactor;

	// Custom interactor style
	vtkSmartPointer<QVTKInteractorStyle> _interactorStyle;

	// IVtk_ShapePicker from OCC VIS
	vtkSmartPointer<IVtkTools_ShapePicker> _shapePicker;

	// Widget for displaying global coordinate system
	vtkSmartPointer<vtkOrientationMarkerWidget> _vtkAxesWidget;

	// Navigation widget
	vtkNew<vtkCameraOrientationWidget> _camOrientManipulator;

	// Water mark
	vtkSmartPointer<vtkLogoWidget> _logoWidget;

	// Geometry selecion pipelines
	std::vector<QIVtkSelectionPipeline> _geometryImportPipelines;
};
};

#endif