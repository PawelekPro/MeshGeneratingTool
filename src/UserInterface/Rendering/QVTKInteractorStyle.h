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

#ifndef QVTKINTERACTORSTYLE_H
#define QVTKINTERACTORSTYLE_H

#include "QIVtkSelectionPipeline.h"
#include "QVTKRenderWindow.h"

// VTK includes
#include <vtkCellPicker.h>
#include <vtkHardwarePicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

// VIS includes
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapeObject.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkTools_SubPolyDataFilter.hxx>

// Qt includes
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
	 * @param  {Rendering::QVTKRenderWindow*} renWin : Render window for which the interactor will be set
	 * @return {QVTKInteractorStyle*}                : Instance of QVTKInteractorStyle class
	 */
	static QVTKInteractorStyle* New();
	vtkTypeMacro(QVTKInteractorStyle, vtkInteractorStyleTrackballCamera);

	/**
	 * @brief  Handle the right mouse button press event.
	 *
	 */
	virtual void OnRightButtonDown() override;

	/**
	 * @brief  Handle the left mouse button press event.
	 *
	 */
	virtual void OnLeftButtonDown() override;

	/**
	 * @brief  Method for handling mouse moving event.
	 *
	 */
	virtual void OnMouseMove() override;

	virtual void OnKeyPress() override;
	virtual void OnKeyRelease() override;

	/**
	 * @brief  Activate interactor style and set its default render window widget.
	 *
	 * @param  {Rendering::QVTKRenderWindow*} renWin : Render window to set interactor style
	 */
	void Initialize(Rendering::QVTKRenderWindow* renWin);

	/**
	 * @brief  Get rendering window instance.
	 *
	 * @return {Rendering::QVTKRenderWindow*}  : Rendering window instance
	 */
	Rendering::QVTKRenderWindow* getRenderWindow();

	void setRenderer(const vtkSmartPointer<vtkRenderer>& theRenderer);
	vtkSmartPointer<vtkRenderer> getRenderer() const;
	void setPicker(const vtkSmartPointer<IVtkTools_ShapePicker>& thePicker);
	vtkSmartPointer<IVtkTools_ShapePicker> getPicker() const;
	void setPipeline(const Handle(QIVtkSelectionPipeline) pipeline);

protected:
	QVTKInteractorStyle();
	~QVTKInteractorStyle();

private:
	QVTKInteractorStyle(const QVTKInteractorStyle&);
	void operator=(const QVTKInteractorStyle&);

private:
	/**
	 * @brief Create context menu on position where mouse right button has been pressed.
	 *
	 */
	void createContextMenu();

private:
	Rendering::QVTKRenderWindow* _qvtkRenderWindow;
	QPointer<QMenu> _contextMenu;
	QPointer<QAction> _fitViewAction;
	QPointer<QAction> _faceSizingAction;
	QPointer<QAction> _edgeSizingAction;

	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<IVtkTools_ShapePicker> m_picker;
	Handle(QIVtkSelectionPipeline) m_pipeline;
};
};

#endif