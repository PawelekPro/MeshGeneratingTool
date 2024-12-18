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

#ifndef QVTKINTERACTORSTYLE_HPP
#define QVTKINTERACTORSTYLE_HPP

// Forward declaration
namespace Rendering {
class QVTKRenderWindow;
};

#include <functional>

#include "QIVtkSelectionPipeline.hpp"
#include "QVTKRenderWindow.hpp"

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
 * @class QVTKInteractorStyle
 * @brief Provides custom interactions with a VTK render window.
 *
 * The QVTKInteractorStyle class defines custom interactions for a VTK render window,
 * including handling mouse and keyboard events, managing selection and highlight pipelines,
 * and integrating with Qt menus.
 */
class QVTKInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
	/**
	 * @brief Creates a new instance of the QVTKInteractorStyle class.
	 * @return A pointer to the new instance of QVTKInteractorStyle.
	 */
	static QVTKInteractorStyle* New();
	vtkTypeMacro(QVTKInteractorStyle, vtkInteractorStyleTrackballCamera);

public:
	/**
	 * @brief Sets the renderer for the interactor style.
	 * @param renderer A smart pointer to the VTK renderer.
	 */
	void setRenderer(const vtkSmartPointer<vtkRenderer>&);

	/**
	 * @brief Sets the QVTKRenderWindow for the interactor style.
	 * @param qvtkRenderWindow A pointer to the QVTKRenderWindow.
	 */
	void setQVTKRenderWindow(const Rendering::QVTKRenderWindow*);

	/**
	 * @brief Gets the renderer associated with the interactor style.
	 * @return A smart pointer to the VTK renderer.
	 */
	vtkSmartPointer<vtkRenderer> getRenderer() const;

	/**
	 * @brief Sets the picker for the interactor style.
	 * @param picker A smart pointer to the IVtkTools_ShapePicker.
	 */
	void setPicker(const vtkSmartPointer<IVtkTools_ShapePicker>&);

	/**
	 * @brief Gets the picker associated with the interactor style.
	 * @return A smart pointer to the IVtkTools_ShapePicker.
	 */
	vtkSmartPointer<IVtkTools_ShapePicker> getPicker() const;

	/**
	 * @brief Adds a selection pipeline to the interactor style.
	 * @param pipeline A handle to the QIVtkSelectionPipeline.
	 * @param id The ID associated with the pipeline.
	 */
	void addPipeline(const Handle(QIVtkSelectionPipeline), IVtk_IdType);

	/**
	 * @brief Sets the selection mode for the interactor style.
	 * @param selectionMode The selection mode to be set.
	 */
	void setSelectionMode(IVtk_SelectionMode);

	/**
	 * @brief Returns the selection mode for the interactor style.
	 */
	IVtk_SelectionMode getSelectionMode();

	/**
	 * @brief Gets the size of the pipelines map.
	 * @return The number of pipelines in the map.
	 */
	Standard_Integer getPipelinesMapSize();
	/**
	 * @brief Returns vector of referenc wrappers to selected TopoDS_Shapes.
	 * 
	 */
	const std::vector<std::reference_wrapper<const TopoDS_Shape>>& getSelectedShapes();

	/**
	 * @brief Gets the list of pipelines.
	 * @return A list of handles to the QIVtkSelectionPipelines.
	 */
	NCollection_List<Handle(QIVtkSelectionPipeline)> getPipelines();
	void removePipelines();

	// Overriding
public:
	/**
	 * @brief  Handle the right mouse button press event.
	 */
	virtual void OnRightButtonDown() override;

	/**
	 * @brief  Handle the left mouse button press event.
	 */
	virtual void OnLeftButtonDown() override;

	/**
	 * @brief  Method for handling mouse moving event.
	 */
	virtual void OnMouseMove() override;

	/**
	 * @brief Handles the key press event.
	 */
	virtual void OnKeyPress() override;

	/**
	 * @brief Handles the key release event.
	 */
	virtual void OnKeyRelease() override;

protected:
	/**
	 * @brief Constructor for QVTKInteractorStyle.
	 * Initializes a new instance of the QVTKInteractorStyle class.
	 */
	QVTKInteractorStyle();

	/**
	 * @brief Destructor for QVTKInteractorStyle.
	 * Cleans up any resources used by the instance.
	 */
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

	/**
	 * @brief Handles moving cursor to specified position.
	 * @param x The X-coordinate.
	 * @param y The Y-coordinate.
	 */
	void MoveTo(Standard_Integer, Standard_Integer);

	/**
	 * @brief Handles the selection process.
	 * @param select Whether to perform multiple selection (default is false).
	 */
	void OnSelection(const Standard_Boolean = Standard_False);



private:
	// ! Pointer to the context menu
	QPointer<QMenu> _contextMenu;

	// ! Pointer to the "Fit View" action.
	QPointer<QAction> _fitViewAction;
	QPointer<QAction> _addSizingAction;
	// QPointer<QAction> _edgeSizingAction;


	// ! Smart pointer to the VTK renderer.
	vtkSmartPointer<vtkRenderer> _renderer;

	// ! Smart pointer to the shape picker.
	vtkSmartPointer<IVtkTools_ShapePicker> _picker;

	// ! Pointer to the QVTK render window.
	const Rendering::QVTKRenderWindow* _qvtkRenderWindow;

	// ! Map of shape pipelines
	ShapePipelinesMap _shapePipelinesMap;

	// ! Map of selected sub-shape IDs.
	SelectedSubShapeIdsMap _selectedSubShapeIdsMap;

	// ! Current selection mode.
	IVtk_SelectionMode _currentSelection;

	std::vector<std::reference_wrapper<const TopoDS_Shape>> _selectedShapes;
};

#endif