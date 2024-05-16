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

	IVtkTools_DisplayModeFilter* aDMFilter
		= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Shape));
	aDMFilter->AddInputConnection(aDataSource->GetOutputPort());
	aDMFilter->SetDisplayMode(IVtk_DisplayMode::DM_Shading);

	myMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	myMapper->AddInputConnection(aDMFilter->GetOutputPort());
	myActor->SetMapper(myMapper);
	IVtkTools_ShapeObject::SetShapeSource(aDataSource, myActor);

	myMapper->ScalarVisibilityOn();
	myMapper->SetScalarModeToUseCellFieldData();

	// Custom lookup table (Optional!)
	vtkSmartPointer<vtkLookupTable> Table = IVtkTools::InitLookupTable();
	IVtkTools::SetLookupTableColor(Table, MT_ShadedFace, 0.35, 0.45, 0.5);
	// FIXME: It seems that setting MT_BoundaryEdge doesnt change anything
	IVtkTools::SetLookupTableColor(Table, MT_BoundaryEdge, 0.0, 0.0, 0.0);

	IVtkTools::InitShapeMapper(myMapper, Table);
	myMapper->Update();

	/* =================================
	 *  Build pipeline for highlighting
	 * ================================= */
	IVtkTools_DisplayModeFilter* aDMFilterH
		= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Hili));
	aDMFilterH->SetDisplayMode(DM_Wireframe);
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