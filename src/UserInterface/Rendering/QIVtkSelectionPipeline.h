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

#ifndef QIVtkSelectionPipeline_H
#define QIVtkSelectionPipeline_H

#include <NCollection_DataMap.hxx>
#include <NCollection_Shared.hxx>
#include <Standard_Transient.hxx>
#include <TopoDS_Shape.hxx>

// prevent disabling some MSVC warning messages by VTK headers
#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_SubPolyDataFilter.hxx>
#include <IVtk_Types.hxx>

typedef NCollection_DataMap<IVtk_IdType, vtkSmartPointer<IVtkTools_DisplayModeFilter>> DisplayModeFiltersMap;
typedef NCollection_DataMap<IVtk_IdType, vtkSmartPointer<IVtkTools_SubPolyDataFilter>> SubShapesFiltersMap;

class QIVtkSelectionPipeline;
DEFINE_STANDARD_HANDLE(QIVtkSelectionPipeline, Standard_Transient)

class QIVtkSelectionPipeline : public Standard_Transient {
public:
	DEFINE_STANDARD_RTTIEXT(QIVtkSelectionPipeline, Standard_Transient)

	QIVtkSelectionPipeline(const TopoDS_Shape& theShape,
		const Standard_Integer theShapeID);
	~QIVtkSelectionPipeline() { }

	void AddToRenderer(vtkRenderer* theRenderer);
	void RemoveFromRenderer(vtkRenderer* theRenderer);
	void ClearHighlightFilters();
	void ClearSelectionFilters();

	IVtkTools_DisplayModeFilter* GetDisplayModeFilter();
	IVtkTools_SubPolyDataFilter* GetHighlightFilter();
	IVtkTools_SubPolyDataFilter* GetSelectionFilter();
	IVtkTools_DisplayModeFilter* GetHighlightDMFilter();
	IVtkTools_DisplayModeFilter* GetSelectionDMFilter();

public:
	inline vtkActor* Actor() { return myActor; }
	inline vtkMapper* Mapper() { return myMapper; }

public:
	//! Filters comprising the pipeline.
	enum FilterId {
		Filter_DM_Shape = 1, //!< Display Mode filter for shape.
		Filter_DM_Hili, //!< Display Mode filter for highlighting.
		Filter_DM_Sel, //!< Display Mode filter for selection.
		Filter_SUB_Hili, //!< Sub-shapes filter for highlighting.
		Filter_SUB_Sel //!< Sub-shapes filter for selection.
	};

private:
	//! Auxiliary map of internal filters by their correspondent IDs.
	typedef NCollection_DataMap<FilterId, vtkSmartPointer<vtkAlgorithm>> FilterMap;

private:
	//! Actor.
	vtkSmartPointer<vtkActor> myActor;

	//! Polygonal mapper.
	vtkSmartPointer<vtkPolyDataMapper> myMapper;

	//! Actor for highlighting.
	vtkSmartPointer<vtkActor> myHiliActor;

	//! Polygonal mapper for highlighting.
	vtkSmartPointer<vtkPolyDataMapper> myHiliMapper;

	//! Actor for selection.
	vtkSmartPointer<vtkActor> mySelActor;

	//! Polygonal mapper for selection.
	vtkSmartPointer<vtkPolyDataMapper> mySelMapper;

	//! Map of involved VTK filters.
	FilterMap myFilterMap;
};

#endif