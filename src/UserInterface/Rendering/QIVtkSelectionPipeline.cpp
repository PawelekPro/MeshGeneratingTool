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
#include <vtkLookupTable.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapeObject.hxx>

IMPLEMENT_STANDARD_RTTIEXT(QIVtkSelectionPipeline, Standard_Transient)

QIVtkSelectionPipeline::QIVtkSelectionPipeline(
	const TopoDS_Shape& theShape, const Standard_Integer theShapeID) {
	/* ===========================
	 *  Allocate involved filters
	 * =========================== */

	myFilterMap.Bind(Filter_DM_Shape, vtkSmartPointer<IVtkTools_DisplayModeFilter>::New());
	myFilterMap.Bind(Filter_DM_Hili, vtkSmartPointer<IVtkTools_DisplayModeFilter>::New());
	myFilterMap.Bind(Filter_DM_Sel, vtkSmartPointer<IVtkTools_DisplayModeFilter>::New());
	myFilterMap.Bind(Filter_SUB_Hili, vtkSmartPointer<IVtkTools_SubPolyDataFilter>::New());
	myFilterMap.Bind(Filter_SUB_Sel, vtkSmartPointer<IVtkTools_SubPolyDataFilter>::New());

	/* ========================
	 *  Build primary pipeline
	 * ======================== */
	myActor = vtkSmartPointer<vtkActor>::New();
	IVtkOCC_Shape::Handle anIVtkShape = new IVtkOCC_Shape(theShape);
	anIVtkShape->SetId(theShapeID);
	vtkSmartPointer<IVtkTools_ShapeDataSource> aDataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
	aDataSource->SetShape(anIVtkShape);

	// Shading / Wireframe display
	// IVtkTools_DisplayModeFilter* aDMFilter
	// 	= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Shape));
	// aDMFilter->AddInputConnection(aDataSource->GetOutputPort());
	// aDMFilter->SetDisplayMode(IVtk_DisplayMode::DM_Shading);

	myMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	myMapper->AddInputConnection(aDataSource->GetOutputPort());
	myActor->SetMapper(myMapper);
	IVtkTools_ShapeObject::SetShapeSource(aDataSource, myActor);

	myMapper->ScalarVisibilityOn();
	myMapper->SetScalarModeToUseCellFieldData();

	// Custom lookup table (Optional!)
	// Set colors table for 3D shapes
	vtkSmartPointer<vtkLookupTable> aColorTable = vtkSmartPointer<vtkLookupTable>::New();
	double aRange[2];
	aRange[0] = IVtk_MeshType::MT_Undefined;
	aRange[1] = IVtk_MeshType::MT_ShadedFace;
	aColorTable->Allocate(9);
	aColorTable->SetNumberOfTableValues(9);
	aColorTable->SetTableRange(aRange);
	aColorTable->SetValueRange(0, 1);

	// Custom colors
	aColorTable->SetTableValue(0, 0, 0, 0); // Undefined
	aColorTable->SetTableValue(1, 0.5, 0.5, 0.5); // gray for IsoLine
	aColorTable->SetTableValue(2, 1, 0, 0); // red for Free vertex
	aColorTable->SetTableValue(3, 0, 0, 0); // Shared vertex
	aColorTable->SetTableValue(4, 1, 0, 0); // red for Free edge
	aColorTable->SetTableValue(5, 0, 1, 0); // green for Boundary edge (related to a single face)
	aColorTable->SetTableValue(6, 0, 0, 0); // Shared edge (related to several faces)
	aColorTable->SetTableValue(7, 1, 1, 0); // yellow for Wireframe face
	aColorTable->SetTableValue(8, 0.35, 0.35, 0.35); // Shaded face

	IVtkTools::InitShapeMapper(myMapper, aColorTable);
	myMapper->Update();

	/* =================================
	 *  Build pipeline for highlighting
	 * ================================= */
	IVtkTools_DisplayModeFilter* aDMFilterH
		= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Hili));
	aDMFilterH->SetDisplayMode(IVtk_DisplayMode::DM_Wireframe);
	IVtkTools_SubPolyDataFilter* aSUBFilterH
		= IVtkTools_SubPolyDataFilter::SafeDownCast(myFilterMap.Find(Filter_SUB_Hili));

	// No highligthing exists initially
	aSUBFilterH->SetInputConnection(aDataSource->GetOutputPort());
	aDMFilterH->SetInputConnection(aSUBFilterH->GetOutputPort());

	myHiliMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	myHiliMapper->SetInputConnection(aDMFilterH->GetOutputPort());

	// Create non-pickable actor
	myHiliActor = vtkSmartPointer<vtkActor>::New();
	myHiliActor->SetPickable(0);
	myHiliActor->SetVisibility(1);
	myHiliActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
	myHiliActor->GetProperty()->SetOpacity(1);
	myHiliActor->GetProperty()->SetPointSize(myHiliActor->GetProperty()->GetPointSize() + 2);
	myHiliActor->GetProperty()->SetLineWidth(myHiliActor->GetProperty()->GetLineWidth() + 1);

	// Set maper for actor
	myHiliActor->SetMapper(myHiliMapper);
	myHiliMapper->ScalarVisibilityOff();

	/* ==============================
	 *  Build pipeline for selection
	 * ============================== */
	IVtkTools_DisplayModeFilter* aDMFilterS
		= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Sel));
	aDMFilterH->SetDisplayMode(IVtk_DisplayMode::DM_Wireframe);
	IVtkTools_SubPolyDataFilter* aSUBFilterS
		= IVtkTools_SubPolyDataFilter::SafeDownCast(myFilterMap.Find(Filter_SUB_Sel));

	// No highligthing exists initially
	aSUBFilterS->SetInputConnection(aDataSource->GetOutputPort());
	aDMFilterS->SetInputConnection(aSUBFilterS->GetOutputPort());

	mySelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mySelMapper->SetInputConnection(aDMFilterS->GetOutputPort());

	// Create non-pickable actor
	mySelActor = vtkSmartPointer<vtkActor>::New();
	mySelActor->SetPickable(0);
	mySelActor->SetVisibility(1);
	mySelActor->GetProperty()->SetColor(0, 1.0, 0); // Green color for selection
	mySelActor->GetProperty()->SetOpacity(1);
	mySelActor->GetProperty()->SetPointSize(myHiliActor->GetProperty()->GetPointSize() + 2);
	mySelActor->GetProperty()->SetLineWidth(myHiliActor->GetProperty()->GetLineWidth() + 1);

	// Set maper for actor
	mySelActor->SetMapper(mySelMapper);
	mySelMapper->ScalarVisibilityOff();
}

void QIVtkSelectionPipeline::AddToRenderer(vtkRenderer* theRenderer) {
	theRenderer->AddActor(myActor);
	theRenderer->AddActor(myHiliActor);
	theRenderer->AddActor(mySelActor);
}

void QIVtkSelectionPipeline::RemoveFromRenderer(vtkRenderer* theRenderer) {
	theRenderer->RemoveActor(myActor);
	theRenderer->RemoveActor(myHiliActor);
	theRenderer->RemoveActor(mySelActor);

	vtkSmartPointer<vtkRenderWindow> aWin = theRenderer->GetRenderWindow();
	if (aWin != NULL) {
		myActor->ReleaseGraphicsResources(aWin);
		myHiliActor->ReleaseGraphicsResources(aWin);
		mySelActor->ReleaseGraphicsResources(aWin);
	}
}

void QIVtkSelectionPipeline::ClearHighlightFilters() {
	this->GetHighlightFilter()->Clear();
	this->GetHighlightFilter()->SetDoFiltering(true);
	this->GetHighlightFilter()->Modified();
}

void QIVtkSelectionPipeline::ClearSelectionFilters() {
	this->GetSelectionFilter()->Clear();
	this->GetSelectionFilter()->SetDoFiltering(true);
	this->GetSelectionFilter()->Modified();
}

IVtkTools_DisplayModeFilter* QIVtkSelectionPipeline::GetDisplayModeFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Shape));
}

IVtkTools_SubPolyDataFilter* QIVtkSelectionPipeline::GetHighlightFilter() {
	return IVtkTools_SubPolyDataFilter::SafeDownCast(myFilterMap.Find(Filter_SUB_Hili));
}

IVtkTools_SubPolyDataFilter* QIVtkSelectionPipeline::GetSelectionFilter() {
	return IVtkTools_SubPolyDataFilter::SafeDownCast(myFilterMap.Find(Filter_SUB_Sel));
}

IVtkTools_DisplayModeFilter* QIVtkSelectionPipeline::GetHighlightDMFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Hili));
}

IVtkTools_DisplayModeFilter* QIVtkSelectionPipeline::GetSelectionDMFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Sel));
}