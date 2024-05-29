/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef QIVtkSelectionPipeline_H
#define QIVtkSelectionPipeline_H

#include "QIVtkUtils.h"

#include <NCollection_Shared.hxx>
#include <Standard_Transient.hxx>
#include <TopoDS_Shape.hxx>

// prevent disabling some MSVC warning messages by VTK headers
#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <vtkActor.h>
#include <vtkAlgorithmOutput.h>
#include <vtkLookupTable.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_SubPolyDataFilter.hxx>
#include <IVtk_Types.hxx>

typedef NCollection_DataMap<IVtk_IdType, vtkSmartPointer<IVtkTools_DisplayModeFilter>> DisplayModeFiltersMap;
typedef NCollection_DataMap<IVtk_IdType, vtkSmartPointer<IVtkTools_SubPolyDataFilter>> SubShapesFiltersMap;

class QIVtkSelectionPipeline;
DEFINE_STANDARD_HANDLE(QIVtkSelectionPipeline, Standard_Transient)

/**
 * @class QIVtkSelectionPipeline
 * @brief Manages the selection and highlighting pipeline for a VTK actor.
 *
 * The QIVtkSelectionPipeline class handles the setup and management of the VTK pipeline
 * for rendering shapes with different display modes and for managing selection and highlighting.
 * It includes methods to add and remove actors from renderers, clear filters, and update the pipeline.
 */
class QIVtkSelectionPipeline : public Standard_Transient {
public:
	/**
	 * @brief Provides run-time type information for QIVtkSelectionPipeline.
	 */
	DEFINE_STANDARD_RTTIEXT(QIVtkSelectionPipeline, Standard_Transient)

	/**
	 * @brief Constructs a QIVtkSelectionPipeline for a given shape and shape ID.
	 * @param theShape The TopoDS_Shape to be managed by this pipeline.
	 * @param theShapeID The ID of the shape.
	 */
	QIVtkSelectionPipeline(const TopoDS_Shape& theShape,
		const Standard_Integer theShapeID);

	/**
	 * @brief Destructor for QIVtkSelectionPipeline.
	 * Cleans up any resources used by the instance.
	 */
	~QIVtkSelectionPipeline() { }

	/**
	 * @brief Adds the actor to the specified renderer.
	 * @param theRenderer The VTK renderer to add the actor to.
	 */
	void AddToRenderer(vtkRenderer* theRenderer);

	/**
	 * @brief Removes the actor from the specified renderer.
	 * @param theRenderer The VTK renderer to remove the actor from.
	 */
	void RemoveFromRenderer(vtkRenderer* theRenderer);

	/**
	 * @brief Clears all highlight filters.
	 */
	void ClearHighlightFilters();

	/**
	 * @brief Clears all selection filters.
	 */
	void ClearSelectionFilters();

	/**
	 * @brief Gets the display mode filter.
	 * @return A pointer to the IVtkTools_DisplayModeFilter.
	 */
	IVtkTools_DisplayModeFilter* GetDisplayModeFilter();

	/**
	 * @brief Gets the highlight filter.
	 * @return A pointer to the IVtkTools_SubPolyDataFilter.
	 */
	IVtkTools_SubPolyDataFilter* GetHighlightFilter();

	/**
	 * @brief Gets the selection filter.
	 * @return A pointer to the IVtkTools_SubPolyDataFilter.
	 */
	IVtkTools_SubPolyDataFilter* GetSelectionFilter();

	/**
	 * @brief Gets the display mode filter used for highlighting.
	 * @return A pointer to the IVtkTools_DisplayModeFilter.
	 */
	IVtkTools_DisplayModeFilter* GetHighlightDMFilter();

	/**
	 * @brief Gets the display mode filter used for selection.
	 * @return A pointer to the IVtkTools_DisplayModeFilter.
	 */
	IVtkTools_DisplayModeFilter* GetSelectionDMFilter();

	/**
	 * @brief Updates the primary pipeline.
	 */
	void updatePrimaryPipeline();

	/**
	 * @brief Updates the primary pipeline with a specified display mode filter.
	 * @param displayModeFilter The display mode filter to be applied.
	 */
	void updatePrimaryPipeline(IVtk_DisplayMode displayModeFilter);

	/**
	 * @brief Updates the primary pipeline with a specified color lookup table.
	 * @param colorsTable The color lookup table to be used.
	 */
	void updatePrimaryPipeline(vtkLookupTable* colorsTable);

public:
	/**
	 * @brief Gets the actor associated with this pipeline.
	 * @return A pointer to the vtkActor.
	 */
	inline vtkActor* Actor() { return _actor; }

	/**
	 * @brief Gets the mapper associated with this pipeline.
	 * @return A pointer to the vtkMapper.
	 */
	inline vtkMapper* Mapper() { return _mapper; }

public:
	/**
	 * @enum FilterId
	 * @brief Enumeration of filter IDs used in the pipeline.
	 */
	enum FilterId {
		Filter_DM_Shape = 1, //!< Display Mode filter for shape.
		Filter_DM_Hili, //!< Display Mode filter for highlighting.
		Filter_DM_Sel, //!< Display Mode filter for selection.
		Filter_SUB_Hili, //!< Sub-shapes filter for highlighting.
		Filter_SUB_Sel //!< Sub-shapes filter for selection.
	};

private:
	/**
	 * @brief Type definition for a map of filters by their IDs.
	 */
	typedef NCollection_DataMap<FilterId, vtkSmartPointer<vtkAlgorithm>> FilterMap;

private:
	//! Shape data source.
	vtkSmartPointer<IVtkTools_ShapeDataSource> _dataSource;

	//! Actor.
	vtkSmartPointer<vtkActor> _actor;

	//! Polygonal mapper.
	vtkSmartPointer<vtkPolyDataMapper> _mapper;

	//! Actor for highlighting.
	vtkSmartPointer<vtkActor> _hiliActor;

	//! Polygonal mapper for highlighting.
	vtkSmartPointer<vtkPolyDataMapper> _hiliMapper;

	//! Actor for selection.
	vtkSmartPointer<vtkActor> _selActor;

	//! Polygonal mapper for selection.
	vtkSmartPointer<vtkPolyDataMapper> _selMapper;

	//! Map of involved VTK filters.
	FilterMap _filterMap;
};

#endif