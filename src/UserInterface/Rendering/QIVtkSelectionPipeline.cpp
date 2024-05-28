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

#include "QIVtkSelectionPipeline.h"

// prevent disabling some MSVC warning messages by VTK headers
#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeObject.hxx>

IMPLEMENT_STANDARD_RTTIEXT(QIVtkSelectionPipeline, Standard_Transient)

//----------------------------------------------------------------------------
QIVtkSelectionPipeline::QIVtkSelectionPipeline(
	const TopoDS_Shape& theShape, const Standard_Integer theShapeID) {
	/* ===========================
	 *  Allocate involved filters
	 * =========================== */

	_filterMap.Bind(Filter_DM_Shape, vtkSmartPointer<IVtkTools_DisplayModeFilter>::New());
	_filterMap.Bind(Filter_DM_Hili, vtkSmartPointer<IVtkTools_DisplayModeFilter>::New());
	_filterMap.Bind(Filter_DM_Sel, vtkSmartPointer<IVtkTools_DisplayModeFilter>::New());
	_filterMap.Bind(Filter_SUB_Hili, vtkSmartPointer<IVtkTools_SubPolyDataFilter>::New());
	_filterMap.Bind(Filter_SUB_Sel, vtkSmartPointer<IVtkTools_SubPolyDataFilter>::New());

	/* ========================
	 *  Build primary pipeline
	 * ======================== */
	_actor = vtkSmartPointer<vtkActor>::New();
	IVtkOCC_Shape::Handle anIVtkShape = new IVtkOCC_Shape(theShape);
	anIVtkShape->SetId(theShapeID);
	_dataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
	_dataSource->SetShape(anIVtkShape);

	_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	_mapper->SetInputConnection(_dataSource->GetOutputPort());
	_actor->SetMapper(_mapper);
	IVtkTools_ShapeObject::SetShapeSource(_dataSource, _actor);

	vtkSmartPointer<QIVtkLookupTable> aColorTable = vtkSmartPointer<QIVtkLookupTable>::New();
	IVtkTools::InitShapeMapper(_mapper, aColorTable->GetAsBaseClass());

	/* =================================
	 *  Build pipeline for highlighting
	 * ================================= */
	IVtkTools_DisplayModeFilter* aDMFilterH
		= IVtkTools_DisplayModeFilter::SafeDownCast(_filterMap.Find(Filter_DM_Hili));
	aDMFilterH->SetDisplayMode(IVtk_DisplayMode::DM_Wireframe);
	IVtkTools_SubPolyDataFilter* aSUBFilterH
		= IVtkTools_SubPolyDataFilter::SafeDownCast(_filterMap.Find(Filter_SUB_Hili));

	// No highligthing exists initially
	aSUBFilterH->SetInputConnection(_dataSource->GetOutputPort());
	aDMFilterH->SetInputConnection(aSUBFilterH->GetOutputPort());

	_hiliMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	_hiliMapper->SetInputConnection(aDMFilterH->GetOutputPort());

	// Create non-pickable actor
	_hiliActor = vtkSmartPointer<vtkActor>::New();
	_hiliActor->SetPickable(0);
	_hiliActor->SetVisibility(1);
	_hiliActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
	_hiliActor->GetProperty()->SetOpacity(1);
	_hiliActor->GetProperty()->SetPointSize(_hiliActor->GetProperty()->GetPointSize() + 2);
	_hiliActor->GetProperty()->SetLineWidth(_hiliActor->GetProperty()->GetLineWidth() + 1);

	// Set maper for actor
	_hiliActor->SetMapper(_hiliMapper);
	_hiliMapper->ScalarVisibilityOff();

	/* ==============================
	 *  Build pipeline for selection
	 * ============================== */
	IVtkTools_DisplayModeFilter* aDMFilterS
		= IVtkTools_DisplayModeFilter::SafeDownCast(_filterMap.Find(Filter_DM_Sel));
	aDMFilterH->SetDisplayMode(IVtk_DisplayMode::DM_Wireframe);
	IVtkTools_SubPolyDataFilter* aSUBFilterS
		= IVtkTools_SubPolyDataFilter::SafeDownCast(_filterMap.Find(Filter_SUB_Sel));

	// No highligthing exists initially
	aSUBFilterS->SetInputConnection(_dataSource->GetOutputPort());
	aDMFilterS->SetInputConnection(aSUBFilterS->GetOutputPort());

	_selMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	_selMapper->SetInputConnection(aDMFilterS->GetOutputPort());

	// Create non-pickable actor
	_selActor = vtkSmartPointer<vtkActor>::New();
	_selActor->SetPickable(0);
	_selActor->SetVisibility(1);
	_selActor->GetProperty()->SetColor(0, 1.0, 0); // Green color for selection
	_selActor->GetProperty()->SetOpacity(1);
	_selActor->GetProperty()->SetPointSize(_hiliActor->GetProperty()->GetPointSize() + 2);
	_selActor->GetProperty()->SetLineWidth(_hiliActor->GetProperty()->GetLineWidth() + 1);

	// Set maper for actor
	_selActor->SetMapper(_selMapper);
	_selMapper->ScalarVisibilityOff();
}

//----------------------------------------------------------------------------
void QIVtkSelectionPipeline::AddToRenderer(vtkRenderer* theRenderer) {
	theRenderer->AddActor(_actor);
	theRenderer->AddActor(_hiliActor);
	theRenderer->AddActor(_selActor);
}

//----------------------------------------------------------------------------
void QIVtkSelectionPipeline::RemoveFromRenderer(vtkRenderer* theRenderer) {
	theRenderer->RemoveActor(_actor);
	theRenderer->RemoveActor(_hiliActor);
	theRenderer->RemoveActor(_selActor);

	vtkSmartPointer<vtkRenderWindow> aWin = theRenderer->GetRenderWindow();
	if (aWin != NULL) {
		_actor->ReleaseGraphicsResources(aWin);
		_hiliActor->ReleaseGraphicsResources(aWin);
		_selActor->ReleaseGraphicsResources(aWin);
	}
}

//----------------------------------------------------------------------------
void QIVtkSelectionPipeline::ClearHighlightFilters() {
	this->GetHighlightFilter()->Clear();
	this->GetHighlightFilter()->SetDoFiltering(true);
	this->GetHighlightFilter()->Modified();
}

//----------------------------------------------------------------------------
void QIVtkSelectionPipeline::ClearSelectionFilters() {
	this->GetSelectionFilter()->Clear();
	this->GetSelectionFilter()->SetDoFiltering(true);
	this->GetSelectionFilter()->Modified();
}

//----------------------------------------------------------------------------
IVtkTools_DisplayModeFilter*
QIVtkSelectionPipeline::GetDisplayModeFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(_filterMap.Find(Filter_DM_Shape));
}

//----------------------------------------------------------------------------
IVtkTools_SubPolyDataFilter*
QIVtkSelectionPipeline::GetHighlightFilter() {
	return IVtkTools_SubPolyDataFilter::SafeDownCast(_filterMap.Find(Filter_SUB_Hili));
}

//----------------------------------------------------------------------------
IVtkTools_SubPolyDataFilter*
QIVtkSelectionPipeline::GetSelectionFilter() {
	return IVtkTools_SubPolyDataFilter::SafeDownCast(_filterMap.Find(Filter_SUB_Sel));
}

//----------------------------------------------------------------------------
IVtkTools_DisplayModeFilter*
QIVtkSelectionPipeline::GetHighlightDMFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(_filterMap.Find(Filter_DM_Hili));
}

//----------------------------------------------------------------------------
IVtkTools_DisplayModeFilter*
QIVtkSelectionPipeline::GetSelectionDMFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(_filterMap.Find(Filter_DM_Sel));
}

//----------------------------------------------------------------------------
void QIVtkSelectionPipeline::updatePrimaryPipeline() {
	_mapper->SetInputConnection(_dataSource->GetOutputPort());
	_mapper->Update();
}

//----------------------------------------------------------------------------
void QIVtkSelectionPipeline::updatePrimaryPipeline(
	IVtk_DisplayMode displayModeFilter) {

	IVtkTools_DisplayModeFilter* aDMFilter
		= IVtkTools_DisplayModeFilter::SafeDownCast(_filterMap.Find(Filter_DM_Shape));
	aDMFilter->SetInputConnection(_dataSource->GetOutputPort());
	aDMFilter->SetDisplayMode(displayModeFilter);

	_mapper->SetInputConnection(aDMFilter->GetOutputPort());
	_mapper->Update();
}

//----------------------------------------------------------------------------
void QIVtkSelectionPipeline::updatePrimaryPipeline(
	vtkLookupTable* colorsTable) {
	IVtkTools::InitShapeMapper(_mapper, colorsTable);
}