#include <vtkObjectFactory.h>

#include "IVtk_CustomInteractorStyle.hxx"

static void ClearHighlightAndSelection(const Handle(IVtk_CustomSelectionPipeline) & thePipeline,
	const Standard_Boolean doHighlighting,
	const Standard_Boolean doSelection) {
	if (!doHighlighting && !doSelection) {
		return;
	}

	if (doHighlighting) {
		thePipeline->ClearHighlightFilters();
	}

	if (doSelection) {
		thePipeline->ClearSelectionFilters();
	}
}

vtkStandardNewMacro(IVtk_CustomInteractorStyle);

IVtk_CustomInteractorStyle::IVtk_CustomInteractorStyle()
	: vtkInteractorStyleTrackballCamera() { }

void IVtk_CustomInteractorStyle::SetRenderer(const vtkSmartPointer<vtkRenderer>& theRenderer) {
	m_renderer = theRenderer;
}

vtkSmartPointer<vtkRenderer> IVtk_CustomInteractorStyle::GetRenderer() const {
	return m_renderer;
}

void IVtk_CustomInteractorStyle::SetPicker(const vtkSmartPointer<IVtkTools_ShapePicker>& thePicker) {
	m_picker = thePicker;
}

vtkSmartPointer<IVtkTools_ShapePicker> IVtk_CustomInteractorStyle::GetPicker() const {
	return m_picker;
}

void IVtk_CustomInteractorStyle::OnLeftButtonDown() {
	m_picker->SetSelectionMode(SM_Face);
	m_picker->SetTolerance(0.025);

	Standard_Integer aPos[2] = { this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1] };
	m_picker->Pick(aPos[0], aPos[1], 0);

	// Traversing results
	vtkActor* aPickedActor = NULL;
	vtkSmartPointer<vtkActorCollection> anActorCollection = m_picker->GetPickedActors();

	if (anActorCollection) {
		// Highlight picked subshapes
		ClearHighlightAndSelection(m_pipeline, Standard_True, Standard_False);
		anActorCollection->InitTraversal();
		while (vtkActor* anActor = anActorCollection->GetNextActor()) {

			IVtkTools_ShapeDataSource* aDataSource = IVtkTools_ShapeObject::GetShapeSource(anActor);
			if (!aDataSource) {
				continue;
			}

			IVtkOCC_Shape::Handle anOccShape = aDataSource->GetShape();
			if (anOccShape.IsNull()) {
				continue;
			}

			IVtk_IdType aShapeID = anOccShape->GetId();
			IVtkTools_SubPolyDataFilter* aFilter = m_pipeline->GetHighlightFilter();
			// Set the selected sub-shapes ids to subpolydata filter.
			IVtk_ShapeIdList aSubShapeIds = m_picker->GetPickedSubShapesIds(aShapeID);

			// Get ids of cells for picked subshapes.
			IVtk_ShapeIdList aSubIds;
			IVtk_ShapeIdList::Iterator aMetaIds(aSubShapeIds);
			for (; aMetaIds.More(); aMetaIds.Next()) {
				IVtk_ShapeIdList aSubSubIds = anOccShape->GetSubIds(aMetaIds.Value());
				aSubIds.Append(aSubSubIds);
				const TopoDS_Shape& aSubShape = anOccShape->GetSubShape(aMetaIds.Value());
				cout << "--------------------------------------------------------------" << endl;
				cout << "Sub-shape ID: " << aMetaIds.Value() << endl;
				cout << "Sub-shape type: " << aSubShape.TShape()->DynamicType()->Name() << endl;
			}
			aFilter->SetDoFiltering(!aSubIds.IsEmpty());
			aFilter->SetData(aSubIds);
			if (!aFilter->GetInput()) {
				aFilter->SetInputConnection(aDataSource->GetOutputPort());
			}
			aFilter->Modified();
		}
	}
	m_pipeline->Mapper()->Update();

	// Invoke basic method
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void IVtk_CustomInteractorStyle::OnMouseMove() {
	// Invoke basic method
	vtkInteractorStyleTrackballCamera::OnMouseMove();
}