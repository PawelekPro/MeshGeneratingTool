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

#include "QVTKInteractorStyle.hpp"

#include <Message.hxx>
#include <Message_Messenger.hxx>

//----------------------------------------------------------------------------
static void ClearHighlightAndSelection(ShapePipelinesMap& theMap,
	const Standard_Boolean doHighlighting, const Standard_Boolean doSelection) {
	if (!doHighlighting && !doSelection) {
		return;
	}

	ShapePipelinesMap::Iterator pIt(theMap);
	for (; pIt.More(); pIt.Next()) {
		const Handle(QIVtkSelectionPipeline)& pipeline = pIt.Value();

		if (doHighlighting) {
			pipeline->ClearHighlightFilters();
		}

		if (doSelection) {
			pipeline->ClearSelectionFilters();
		}
	}
}

//----------------------------------------------------------------------------
vtkStandardNewMacro(QVTKInteractorStyle);

//----------------------------------------------------------------------------
QVTKInteractorStyle::QVTKInteractorStyle()
	: _contextMenu(nullptr) { }

//----------------------------------------------------------------------------
void QVTKInteractorStyle::setQVTKRenderWindow(
	const Rendering::QVTKRenderWindow* qvtkRenderWindow) {
	_qvtkRenderWindow = qvtkRenderWindow;
}

//----------------------------------------------------------------------------
QVTKInteractorStyle::~QVTKInteractorStyle() {
	if (_contextMenu)
		_contextMenu->deleteLater();

	ShapePipelinesMap::Iterator pIt(_shapePipelinesMap);
	for (; pIt.More(); pIt.Next()) {
		const Handle(QIVtkSelectionPipeline)& pipeline = pIt.Value();
		if (pipeline)
			pipeline->Delete();
	}

	SelectedSubShapeIdsMap::Iterator sIt(_selectedSubShapeIdsMap);
	for (; sIt.More(); sIt.Next()) {
		const IVtk_ShapeIdList* shapeIdList = sIt.Value();
		if (shapeIdList)
			delete shapeIdList;
	}
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::setRenderer(
	const vtkSmartPointer<vtkRenderer>& theRenderer) {
	_renderer = theRenderer;
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkRenderer> QVTKInteractorStyle::getRenderer() const {
	return _renderer;
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::setPicker(
	const vtkSmartPointer<IVtkTools_ShapePicker>& thePicker) {
	_picker = thePicker;
}

//----------------------------------------------------------------------------
vtkSmartPointer<IVtkTools_ShapePicker>
QVTKInteractorStyle::getPicker() const {
	return _picker;
}

//----------------------------------------------------------------------------
NCollection_List<Handle(QIVtkSelectionPipeline)> QVTKInteractorStyle::getPipelines() {
	NCollection_List<Handle(QIVtkSelectionPipeline)> pipelineList;
	for (ShapePipelinesMap::Iterator it(_shapePipelinesMap); it.More(); it.Next()) {

		const Handle(QIVtkSelectionPipeline)& pipeline = it.Value();
		pipelineList.Append(pipeline);
	}

	return pipelineList;
}
void QVTKInteractorStyle::removePipelines() {
    for (ShapePipelinesMap::Iterator it(_shapePipelinesMap); it.More(); it.Next()) {
        const Handle(QIVtkSelectionPipeline)& pipeline = it.Value();
		pipeline->Delete();
    }
    _shapePipelinesMap.Clear();
}
//----------------------------------------------------------------------------
Standard_Integer QVTKInteractorStyle::getPipelinesMapSize() {
	return _shapePipelinesMap.Size();
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::addPipeline(
	const Handle(QIVtkSelectionPipeline) pipeline, IVtk_IdType shapeID) {
	_shapePipelinesMap.Bind(shapeID, pipeline);
	_selectedSubShapeIdsMap.Bind(shapeID, new IVtk_ShapeIdList());
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::setSelectionMode(
	IVtk_SelectionMode mode) {

	if (_shapePipelinesMap.IsEmpty())
		return;

	// Clear current selection
	SelectedSubShapeIdsMap::Iterator sIt(_selectedSubShapeIdsMap);
	for (; sIt.More(); sIt.Next()) {
		IVtk_ShapeIdList* selectedSubShapeIds = sIt.Value();
		selectedSubShapeIds->Clear();
	}

	ClearHighlightAndSelection(
		_shapePipelinesMap, Standard_True, Standard_True);

	ShapePipelinesMap::Iterator pIt(_shapePipelinesMap);
	for (; pIt.More(); pIt.Next()) {
		const Handle(QIVtkSelectionPipeline)& pipeline = pIt.Value();

		// Deactivate all current selection modes
		IVtk_SelectionModeList modeList
			= _picker->GetSelectionModes(pipeline->Actor());

		for (IVtk_SelectionMode selMode : modeList) {
			_picker->SetSelectionMode(selMode, false);
		}
	}
	// Set given selection mode
	_picker->SetSelectionMode(mode, true);
	_currentSelection = mode;
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::createContextMenu() {
	if (!_contextMenu) {
		_contextMenu = new QMenu;

		// Set font size
		QFont font;
		font.setPointSize(10);
		_contextMenu->setFont(font);

		_fitViewAction = new QAction("Fit view", _contextMenu);
		QObject::connect(_fitViewAction, &QAction::triggered,
			[this]() { _qvtkRenderWindow->fitView(); });

		_addSizingAction = new QAction("Add sizing", _contextMenu);
		QObject::connect(_addSizingAction, &QAction::triggered, 
			[this]() {_qvtkRenderWindow->model->addSizing(this->_selectedShapes);});

		_contextMenu->addAction(_fitViewAction);
		_contextMenu->addAction(_addSizingAction);

	}
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::OnRightButtonDown() {
	this->createContextMenu();
	_contextMenu->exec(QCursor::pos());

	// this->Superclass::OnRightButtonDown();
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::OnLeftButtonDown() {

	if (this->Interactor->GetShiftKey()) {
		// Append new selection to the current one
		this->OnSelection(Standard_True);
	} else {
		// Clear previous selection
		this->OnSelection(Standard_False);
	}

	// Invoke base class event
	this->Superclass::OnLeftButtonDown();
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::OnMouseMove() {
	Standard_Integer aPos[2] = { this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1] };

	this->MoveTo(aPos[0], aPos[1]);
	this->Superclass::OnMouseMove();
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::OnKeyPress() {
	vtkRenderWindowInteractor* rwi = this->Interactor;
	std::string key = rwi->GetKeySym();

	// Clear current selection when Escape is pressed
	if (key == "Escape") {
		SelectedSubShapeIdsMap::Iterator sIt(_selectedSubShapeIdsMap);
		for (; sIt.More(); sIt.Next()) {
			IVtk_ShapeIdList* selectedSubShapeIds = sIt.Value();
			selectedSubShapeIds->Clear();
		}

		ClearHighlightAndSelection(_shapePipelinesMap, Standard_False, Standard_True);

		ShapePipelinesMap::Iterator pIt(_shapePipelinesMap);
		for (; pIt.More(); pIt.Next()) {
			const Handle(QIVtkSelectionPipeline)& pipeline = pIt.Value();
			pipeline->Mapper()->Update();
		}
		_selectedShapes.clear();
	}

	this->Superclass::OnKeyPress();
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::OnKeyRelease() {
	this->Superclass::OnKeyRelease();
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::MoveTo(
	Standard_Integer theX, Standard_Integer theY) {

	_picker->Pick(theX, theY, 0);

	// Traversing results
	vtkSmartPointer<vtkActorCollection> anActorCollection = _picker->GetPickedActors();

	if (anActorCollection) {

		// Highlight picked subshapes
		if (_shapePipelinesMap.IsEmpty())
			return;

		ClearHighlightAndSelection(_shapePipelinesMap, Standard_True, Standard_False);

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

			Handle(Message_Messenger) anOutput = Message::DefaultMessenger();
			if (!_shapePipelinesMap.IsBound(aShapeID)) {
				anOutput->SendWarning()
					<< "Warning: there is no VTK pipeline registered for picked shape"
					<< std::endl;
				continue;
			}

			const Handle(QIVtkSelectionPipeline)& pipeline
				= _shapePipelinesMap.Find(aShapeID);
			IVtk_ShapeIdList* selectedSubShapeIds
				= _selectedSubShapeIdsMap.Find(aShapeID);

			IVtkTools_SubPolyDataFilter* aFilter = pipeline->GetHighlightFilter();

			// Set the selected sub-shapes ids to subpolydata filter.
			IVtk_ShapeIdList aSubShapeIds = _picker->GetPickedSubShapesIds(aShapeID);

			// If picked shape is in selected shapes then do not highlight it
			for (auto shapeID : aSubShapeIds) {
				if (selectedSubShapeIds->Contains(shapeID)) {
					return;
				}
			}

			// Get ids of cells for picked subshapes.
			IVtk_ShapeIdList aSubIds;
			IVtk_ShapeIdList::Iterator aMetaIds(aSubShapeIds);
			for (; aMetaIds.More(); aMetaIds.Next()) {
				IVtk_ShapeIdList aSubSubIds = anOccShape->GetSubIds(aMetaIds.Value());
				aSubIds.Append(aSubSubIds);
				// const TopoDS_Shape& aSubShape = anOccShape->GetSubShape(aMetaIds.Value());
				// cout << "--------------------------------------------------------------" << endl;
				// cout << "Sub-shape ID: " << aMetaIds.Value() << endl;
				// cout << "Sub-shape type: " << aSubShape.TShape()->DynamicType()->Name() << endl;
			}
			aFilter->SetDoFiltering(!aSubIds.IsEmpty());
			aFilter->SetData(aSubIds);
			if (!aFilter->GetInput()) {
				aFilter->SetInputConnection(aDataSource->GetOutputPort());
			}
			aFilter->Modified();

			pipeline->Mapper()->Update();
		}
	}
}

//----------------------------------------------------------------------------
void QVTKInteractorStyle::OnSelection(const Standard_Boolean appendId) {
	vtkSmartPointer<vtkActorCollection> anActorCollection
		= _picker->GetPickedActors();

	if (anActorCollection) {
		if (anActorCollection->GetNumberOfItems() != 0) {
			// Clear previous selection.
			ClearHighlightAndSelection(
				_shapePipelinesMap, Standard_False, Standard_True);
		}

		anActorCollection->InitTraversal();
		while (vtkActor* anActor = anActorCollection->GetNextActor()) {
			IVtkTools_ShapeDataSource* aDataSource
				= IVtkTools_ShapeObject::GetShapeSource(anActor);
			if (!aDataSource) {
				continue;
			}

			IVtkOCC_Shape::Handle anOccShape = aDataSource->GetShape();
			if (anOccShape.IsNull()) {
				continue;
			}

			IVtk_IdType aShapeID = anOccShape->GetId();
			
			Handle(Message_Messenger) anOutput = Message::DefaultMessenger();
			if (!_shapePipelinesMap.IsBound(aShapeID)) {
				anOutput->SendWarning()
					<< "Warning: there is no VTK pipeline registered for picked shape"
					<< std::endl;
				continue;
			}

			const Handle(QIVtkSelectionPipeline)& pipeline
				= _shapePipelinesMap.Find(aShapeID);
			IVtk_ShapeIdList* selectedSubShapeIds
				= _selectedSubShapeIdsMap.Find(aShapeID);

			IVtkTools_SubPolyDataFilter* aFilter = pipeline->GetSelectionFilter();

			// Set the selected sub-shapes ids to subpolydata filter.
			IVtk_ShapeIdList aSubShapeIds;
			if (_currentSelection == IVtk_SelectionMode::SM_Shape) {
				aSubShapeIds = _picker->GetPickedShapesIds(Standard_True);
			} else {
				aSubShapeIds = _picker->GetPickedSubShapesIds(aShapeID);
			}

			if (!appendId) {
				_selectedShapes.clear();
				selectedSubShapeIds->Clear();
			}

			for (auto shapeID : aSubShapeIds) {
				if (!selectedSubShapeIds->Contains(shapeID)) {
					// If selected Ids list does not contain shape then append it.
					selectedSubShapeIds->Append(aSubShapeIds);
				} else {
					// Selecting the shape again causes deselecting it.
					selectedSubShapeIds->Remove(shapeID);
				}
			}

			// If selected Ids list is empty then any selection will not be made
			if (selectedSubShapeIds->IsEmpty()) {
				return;
			}

			// Get ids of cells for picked subshapes
			IVtk_ShapeIdList aSubIds;
			IVtk_ShapeIdList::Iterator aMetaIds(*selectedSubShapeIds);
			_selectedShapes.clear();
			for (; aMetaIds.More(); aMetaIds.Next()) {
				IVtk_ShapeIdList aSubSubIds = anOccShape->GetSubIds(aMetaIds.Value());
				const TopoDS_Shape& aSubShape = anOccShape->GetSubShape(aMetaIds.Value());
				_selectedShapes.push_back(aSubShape);
				aSubIds.Append(aSubSubIds);
			}

			aFilter->SetDoFiltering(!aSubIds.IsEmpty());
			aFilter->SetData(aSubIds);
			if (!aFilter->GetInput()) {
				aFilter->SetInputConnection(aDataSource->GetOutputPort());
			}
			aFilter->Modified();

			if (!pipeline.IsNull())
				pipeline->Mapper()->Update();
		}
	}
}

const std::vector<std::reference_wrapper<const TopoDS_Shape>>& QVTKInteractorStyle::getSelectedShapes(){
	return _selectedShapes;
};