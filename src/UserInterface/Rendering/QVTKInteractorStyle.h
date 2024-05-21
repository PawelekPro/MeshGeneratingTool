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

// Forward declaration
namespace Rendering {
class QVTKRenderWindow;
};

#include "QIVtkSelectionPipeline.h"
#include "QVTKRenderWindow.h"

// VTK includes
#include <vtkCellPicker.h>
#include <vtkHardwarePicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
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
#include <QPointer>

typedef NCollection_DataMap<IVtk_IdType, Handle(QIVtkSelectionPipeline)> ShapePipelinesMap;
typedef NCollection_DataMap<IVtk_IdType, IVtk_ShapeIdList*> SelectedSubShapeIdsMap;

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

public:
	void setRenderer(const vtkSmartPointer<vtkRenderer>&);
	void setQVTKRenderWindow(const Rendering::QVTKRenderWindow* qvtkRenderWindow) { _qvtkRenderWindow = qvtkRenderWindow; }
	vtkSmartPointer<vtkRenderer> getRenderer() const;
	void setPicker(const vtkSmartPointer<IVtkTools_ShapePicker>&);
	vtkSmartPointer<IVtkTools_ShapePicker> getPicker() const;
	void addPipeline(const Handle(QIVtkSelectionPipeline), IVtk_IdType);
	void setSelectionMode(IVtk_SelectionMode);
	Standard_Integer getPipelinesMapSize() { return _shapePipelinesMap.Size(); };
	NCollection_List<Handle(QIVtkSelectionPipeline)> getPipelines();

	// Overriding
public:
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

	void MoveTo(Standard_Integer, Standard_Integer);
	void OnSelection(const Standard_Boolean = Standard_False);

private:
	QPointer<QMenu> _contextMenu;
	QPointer<QAction> _fitViewAction;
	// QPointer<QAction> _edgeSizingAction;

	vtkSmartPointer<vtkRenderer> _renderer;
	vtkSmartPointer<IVtkTools_ShapePicker> _picker;
	const Rendering::QVTKRenderWindow* _qvtkRenderWindow;

	ShapePipelinesMap _shapePipelinesMap;
	SelectedSubShapeIdsMap _selectedSubShapeIdsMap;
	IVtk_SelectionMode _currentSelection;
};

#endif