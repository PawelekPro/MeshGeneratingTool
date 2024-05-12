#include "IVtk_CustomSelectionPipeline.hxx"

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

IMPLEMENT_STANDARD_RTTIEXT(IVtk_CustomSelectionPipeline, Standard_Transient)

IVtk_CustomSelectionPipeline::IVtk_CustomSelectionPipeline(
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

	IVtkTools_DisplayModeFilter*
		aDMFilter
		= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Shape));
	aDMFilter->AddInputConnection(aDataSource->GetOutputPort());
	aDMFilter->SetDisplayMode(DM_Shading);

	myMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	myMapper->AddInputConnection(aDMFilter->GetOutputPort());
	myActor->SetMapper(myMapper);
	IVtkTools_ShapeObject::SetShapeSource(aDataSource, myActor);

	myMapper->ScalarVisibilityOff();
	// myMapper->SetScalarModeToUseCellFieldData();

	// Custom lookup table (optinal!)
	vtkSmartPointer<vtkLookupTable> Table = IVtkTools::InitLookupTable();
	IVtkTools::SetLookupTableColor(Table, MT_ShadedFace, 0.35, 0.45, 0.5);

	IVtkTools::InitShapeMapper(myMapper, Table);
	myMapper->Update();

	/* =================================
	 *  Build pipeline for highlighting
	 * ================================= */
	IVtkTools_DisplayModeFilter*
		aDMFilterH
		= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Hili));
	aDMFilterH->SetDisplayMode(DM_Shading);
	IVtkTools_SubPolyDataFilter*
		aSUBFilterH
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
	myHiliActor->GetProperty()->SetColor(0, 1, 1);
	myHiliActor->GetProperty()->SetOpacity(1);
	myHiliActor->GetProperty()->SetPointSize(myHiliActor->GetProperty()->GetPointSize() + 4);
	myHiliActor->GetProperty()->SetLineWidth(myHiliActor->GetProperty()->GetLineWidth() + 2);

	// Set maper for actor
	myHiliActor->SetMapper(myHiliMapper);
	myHiliMapper->ScalarVisibilityOff();

	/* ==============================
	 *  Build pipeline for selection
	 * ============================== */
	IVtkTools_DisplayModeFilter*
		aDMFilterS
		= IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Sel));
	IVtkTools_SubPolyDataFilter*
		aSUBFilterS
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
	mySelActor->GetProperty()->SetColor(1, 1, 1);
	mySelActor->GetProperty()->SetOpacity(1);
	mySelActor->GetProperty()->SetPointSize(myHiliActor->GetProperty()->GetPointSize() + 4);
	mySelActor->GetProperty()->SetLineWidth(myHiliActor->GetProperty()->GetLineWidth() + 2);

	// Set maper for actor
	mySelActor->SetMapper(mySelMapper);
	mySelMapper->ScalarVisibilityOff();
}

void IVtk_CustomSelectionPipeline::AddToRenderer(vtkRenderer* theRenderer) {
	theRenderer->AddActor(myActor);
	theRenderer->AddActor(myHiliActor);
	theRenderer->AddActor(mySelActor);
}

void IVtk_CustomSelectionPipeline::RemoveFromRenderer(vtkRenderer* theRenderer) {
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

void IVtk_CustomSelectionPipeline::ClearHighlightFilters() {
	this->GetHighlightFilter()->Clear();
	this->GetHighlightFilter()->SetDoFiltering(true);
	this->GetHighlightFilter()->Modified();
}

void IVtk_CustomSelectionPipeline::ClearSelectionFilters() {
	this->GetSelectionFilter()->Clear();
	this->GetSelectionFilter()->SetDoFiltering(true);
	this->GetSelectionFilter()->Modified();
}

IVtkTools_DisplayModeFilter* IVtk_CustomSelectionPipeline::GetDisplayModeFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Shape));
}

IVtkTools_SubPolyDataFilter* IVtk_CustomSelectionPipeline::GetHighlightFilter() {
	return IVtkTools_SubPolyDataFilter::SafeDownCast(myFilterMap.Find(Filter_SUB_Hili));
}

IVtkTools_SubPolyDataFilter* IVtk_CustomSelectionPipeline::GetSelectionFilter() {
	return IVtkTools_SubPolyDataFilter::SafeDownCast(myFilterMap.Find(Filter_SUB_Sel));
}

IVtkTools_DisplayModeFilter* IVtk_CustomSelectionPipeline::GetHighlightDMFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Hili));
}

IVtkTools_DisplayModeFilter* IVtk_CustomSelectionPipeline::GetSelectionDMFilter() {
	return IVtkTools_DisplayModeFilter::SafeDownCast(myFilterMap.Find(Filter_DM_Sel));
}