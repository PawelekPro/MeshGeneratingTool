/* TRASH FILE - MAY BE USEFUL IN THE FUTURE */

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

#include "QVTKToolBar.h"

#include <vtkButtonRepresentation.h>
#include <vtkButtonWidget.h>
#include <vtkCoordinate.h>
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkPNGReader.h>
#include <vtkProperty.h>
#include <vtkTexturedButtonRepresentation2D.h>

vtkStandardNewMacro(ToolBar::QVTKToolBarRepresentation);
vtkStandardNewMacro(ToolBar::QVTKToolBar);

ToolBar::QVTKToolBarRepresentation::QVTKToolBarRepresentation() {
	this->InteractionState = ToolBar::to_underlying(InteractionStateType::Outside);
	this->PickingManaged = true;

	// Ustawianie wymiarów
	this->containerSource->SetOrigin(this->Padding[0] - this->Size[0], this->Padding[1] - this->Size[1], 0.0);
	this->containerSource->SetPoint1(this->Size[0], this->Padding[1] - this->Size[1], 0.0);
	this->containerSource->SetPoint2(this->Padding[0] - this->Size[0], this->Size[1] + this->Padding[1], 0.0);

	this->createDefaultProperties();
}

void ToolBar::QVTKToolBarRepresentation::createDefaultGeometry(vtkRenderWindowInteractor* iren) {

	this->addButton(iren);
}

void ToolBar::QVTKToolBarRepresentation::addButton(vtkRenderWindowInteractor* iren) {
	buttonRepresentation = vtkSmartPointer<vtkTexturedButtonRepresentation2D>::New();
	buttonWidget = vtkSmartPointer<vtkButtonWidget>::New();

	buttonRepresentation->SetNumberOfStates(1);

	vtkNew<vtkPNGReader> pngReader;
	pngReader->SetFileName("image.png");
	pngReader->Update();

	// Create the widget and its representation

	buttonRepresentation->SetButtonTexture(0, pngReader->GetOutput());

	buttonWidget->SetInteractor(iren);
	buttonWidget->SetRepresentation(buttonRepresentation);

	double bounds[6] = { 5, 35, 1000, 1030, 0.0, 0.0 };

	// Scale to 1, default is .5
	buttonRepresentation->SetPlaceFactor(1);
	buttonRepresentation->PlaceWidget(bounds);
}

void ToolBar::QVTKToolBarRepresentation::createDefaultProperties() {
	vtkNew<vtkPolyDataMapper> containerMapper;
	containerMapper->SetInputConnection(this->containerSource->GetOutputPort());
	this->container->SetMapper(containerMapper);
	this->container->SetVisibility(true);

	this->container->GetProperty()->SetColor(0.2, 0.2, 0.2);
	this->container->GetProperty()->SetOpacity(0.1);
	this->container->GetProperty()->SetAmbient(1);
	this->container->GetProperty()->SetDiffuse(0);
}

//-----------------------------------------------------------------------------
void ToolBar::QVTKToolBarRepresentation::ShallowCopy(vtkProp* prop) {
	ToolBar::QVTKToolBarRepresentation* a = ToolBar::QVTKToolBarRepresentation::SafeDownCast(prop);
	if (a != nullptr) {
		a->container->ShallowCopy(this->container);
	}

	// Now do superclass
	this->Superclass::ShallowCopy(prop);
}

void ToolBar::QVTKToolBarRepresentation::ApplyInteractionState(const InteractionStateType& state) {
	// Depending on state, show/hide parts of representation
	switch (state) {
	case InteractionStateType::Hovering:
		this->container->SetVisibility(true);
		this->container->GetProperty()->SetOpacity(0.2);
		break;
	default: // outside
		this->container->SetVisibility(false);
		this->container->GetProperty()->SetOpacity(0.1);
		break;
	}
	this->InteractionState = to_underlying(state);
}

//-----------------------------------------------------------------------------
void ToolBar::QVTKToolBarRepresentation::ApplyInteractionState(const int& state) {
	// Clamp to allowable values
	const int clamped = state < 0 ? 0 : (state > 2 ? 2 : state);
	this->ApplyInteractionState(static_cast<InteractionStateType>(clamped));
}

ToolBar::QVTKToolBarRepresentation::~QVTKToolBarRepresentation() = default;

void ToolBar::QVTKToolBarRepresentation::BuildRepresentation() {
	this->BuildTime.Modified();
}

int ToolBar::QVTKToolBarRepresentation::ComputeInteractionState(int X, int Y, int modify) {
	if (modify) {
		if (!this->Renderer || !this->Renderer->IsInViewport(X, Y)) {
			this->InteractionState = ToolBar::to_underlying(InteractionStateType::Outside);
		} else {
			this->InteractionState = ToolBar::to_underlying(InteractionStateType::Hovering);
		}
	}
	// ToDo: picking?
	return this->InteractionState;
}

void ToolBar::QVTKToolBarRepresentation::StartWidgetInteraction(double eventPos[2]) {
	this->StartEventPosition[0] = eventPos[0];
	this->StartEventPosition[1] = eventPos[1];
	this->StartEventPosition[2] = 0;

	this->LastEventPosition[0] = eventPos[0];
	this->LastEventPosition[1] = eventPos[1];
	this->LastEventPosition[2] = 0;
}

void ToolBar::QVTKToolBarRepresentation::WidgetInteraction(double newEventPos[2]) {
	if (this->Renderer == nullptr) {
		return;
	}
	this->LastEventPosition[0] = newEventPos[0];
	this->LastEventPosition[1] = newEventPos[1];
	this->LastEventPosition[2] = 0;
}

void ToolBar::QVTKToolBarRepresentation::EndWidgetInteraction(double newEventPos[2]) {
	// ToDo
	this->LastEventPosition[0] = newEventPos[0];
	this->LastEventPosition[1] = newEventPos[1];
	this->LastEventPosition[2] = 0;
}

double* ToolBar::QVTKToolBarRepresentation::GetBounds() {
	vtkBoundingBox bbox;
	bbox.SetBounds(this->container->GetBounds());

	bbox.GetBounds(this->Bounds);
	return this->Bounds;
}

int ToolBar::QVTKToolBarRepresentation::RenderOpaqueGeometry(vtkViewport* vp) {
	this->BuildRepresentation();

	int count = 0;

	if (this->container->GetVisibility()) {
		this->container->SetPropertyKeys(this->GetPropertyKeys());
		this->container->GetMapper()->Update();
		count += this->container->RenderOpaqueGeometry(vp);
	}

	return count;
}

//-----------------------------------------------------------------------------
int ToolBar::QVTKToolBarRepresentation::RenderTranslucentPolygonalGeometry(vtkViewport* vp) {
	int count = 0;

	if (this->container->GetVisibility()) {
		this->container->SetPropertyKeys(this->GetPropertyKeys());
		this->container->GetMapper()->Update();
		count += this->container->RenderTranslucentPolygonalGeometry(vp);
	}

	return count;
}

//-----------------------------------------------------------------------------
vtkTypeBool ToolBar::QVTKToolBarRepresentation::HasTranslucentPolygonalGeometry() {
	this->container->GetMapper()->Update();
	int count = 0;
	if (this->container->GetVisibility()) {
		count |= this->container->HasTranslucentPolygonalGeometry();
	}
	return count;
}

//-----------------------------------------------------------------------------
void ToolBar::QVTKToolBarRepresentation::ReleaseGraphicsResources(vtkWindow* win) {
	this->container->ReleaseGraphicsResources(win);
}

/*********************************************************************************************/
ToolBar::QVTKToolBar::QVTKToolBar() {

	// Define widget events.
	// this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent, vtkEvent::NoModifier, 0,
	// 	0, nullptr, vtkWidgetEvent::Select, this, ToolBar::QVTKToolBar::SelectAction);
	// this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent, vtkEvent::NoModifier,
	// 	0, 0, nullptr, vtkWidgetEvent::EndSelect, this, ToolBar::QVTKToolBar::EndSelectAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent, vtkEvent::NoModifier, 0, 0,
		nullptr, vtkWidgetEvent::Rotate, this, ToolBar::QVTKToolBar::MoveAction);

	vtkNew<vtkRenderer> renderer;
	this->SetDefaultRenderer(renderer);
	renderer->SetViewport(0.0, 0.8, 0.025, 1.0);
	renderer->GetActiveCamera()->ParallelProjectionOff();
	renderer->GetActiveCamera()->Dolly(0.25);
	renderer->InteractiveOff();

	renderer->SetLayer(1);
	// renderer->AddObserver(
	// 	vtkCommand::StartEvent, this, &ToolBar::QVTKToolBar::ToolBarWidgetRepresentation);
	// ToDo
}

void ToolBar::QVTKToolBar::ToolBarWidgetRepresentation() {
	if (this->ParentRenderer == nullptr) {
		return;
	}
	auto rep = ToolBar::QVTKToolBarRepresentation::SafeDownCast(this->WidgetRep);
	if (rep == nullptr) {
		return;
	}
}

void ToolBar::QVTKToolBar::CreateDefaultRepresentation() {
	if (!this->WidgetRep) {
		this->WidgetRep = ToolBar::QVTKToolBarRepresentation::New();
	}
}

void ToolBar::QVTKToolBar::SetParentRenderer(vtkRenderer* parentRen) {
	if (this->ParentRenderer == parentRen) {
		return;
	}
	if (this->ParentRenderer != nullptr) {
		auto renWin = this->ParentRenderer->GetRenderWindow();
		if (renWin != nullptr) {
			if (renWin->HasRenderer(this->DefaultRenderer)) {
				renWin->RemoveRenderer(this->DefaultRenderer);
			}
			const int& numLayers = renWin->GetNumberOfLayers();
			renWin->SetNumberOfLayers(numLayers - 1);
			renWin->RemoveObserver(this->ResizeObserverTag);
		}
	}

	// attach to given parent.
	if (parentRen != nullptr) {
		auto renWin = parentRen->GetRenderWindow();
		if (renWin != nullptr) {
			if (!renWin->HasRenderer(this->DefaultRenderer)) {
				renWin->AddRenderer(this->DefaultRenderer);
			}
			this->SetInteractor(renWin->GetInteractor());
			const int& numLayers = renWin->GetNumberOfLayers();
			renWin->SetNumberOfLayers(numLayers + 1);
			// ToDo: Resizing event
		}
	}

	// assign
	this->ParentRenderer = parentRen;
	this->Modified();
}

void ToolBar::QVTKToolBar::ComputeWidgetState(int X, int Y, int modify) {
	auto rep = ToolBar::QVTKToolBarRepresentation::SafeDownCast(this->WidgetRep);
	if (rep == nullptr) {
		return;
	}

	// Compute and get representation's interaction state.
	this->WidgetRep->ComputeInteractionState(X, Y, modify);

	const auto& interactionState = rep->GetInteractionStateAsEnum();

	// Synchronize widget state with representation.
	if (interactionState == ToolBar::QVTKToolBarRepresentation::InteractionStateType::Outside) {
		this->WidgetState = WidgetStateType::Inactive;
	} else if (interactionState == ToolBar::QVTKToolBarRepresentation::InteractionStateType::Hovering) {
		this->WidgetState = WidgetStateType::Hot;
	}

	// Refresh representation to match interaction state.
	rep->ApplyInteractionState(interactionState);
}

void ToolBar::QVTKToolBar::EndSelectAction(vtkAbstractWidget* widget) {
	// cast to itself
	ToolBar::QVTKToolBar* const self = ToolBar::QVTKToolBar::SafeDownCast(widget);
	if (self == nullptr) {
		return;
	}
	auto rep = ToolBar::QVTKToolBarRepresentation::SafeDownCast(self->WidgetRep);
	if (rep == nullptr) {
		return;
	}

	// can only be deselected if already selected
	if (self->WidgetState != WidgetStateType::Active) {
		return;
	}

	if (self->ParentRenderer == nullptr) {
		return;
	}

	// get event position.
	const int& X = self->Interactor->GetEventPosition()[0];
	const int& Y = self->Interactor->GetEventPosition()[1];

	// end widget interaction.
	double e[2];
	e[0] = static_cast<double>(X);
	e[1] = static_cast<double>(Y);
	self->WidgetRep->EndWidgetInteraction(e);

	// deactivate widget.
	self->WidgetState = WidgetStateType::Inactive;

	self->ParentRenderer->ResetCamera();
	self->Render();

	self->ComputeWidgetState(X, Y, 1);

	self->ReleaseFocus();
	self->EventCallbackCommand->AbortFlagOn();
	self->EndInteraction();
	self->InvokeEvent(vtkCommand::EndInteractionEvent);
	self->Render();
}

void ToolBar::QVTKToolBar::MoveAction(vtkAbstractWidget* widget) {
	// cast to ourself
	ToolBar::QVTKToolBar* const self = ToolBar::QVTKToolBar::SafeDownCast(widget);
	if (self == nullptr) {
		return;
	}
	auto rep = ToolBar::QVTKToolBarRepresentation::SafeDownCast(self->WidgetRep);
	if (rep == nullptr) {
		return;
	}

	// Get event position.
	const int& X = self->Interactor->GetEventPosition()[0];
	const int& Y = self->Interactor->GetEventPosition()[1];

	// can only rotate if previously selected, else simply compute widget state.
	if (self->WidgetState != WidgetStateType::Active) {
		self->ComputeWidgetState(X, Y, 1);
	} else // pick handle.
	{
		rep->ComputeInteractionState(X, Y, 0);
		if (self->ParentRenderer == nullptr) {
			return;
		}
		auto cam = self->ParentRenderer->GetActiveCamera();
		if (cam == nullptr) {
			return;
		}

		double e[2];
		e[0] = static_cast<double>(X);
		e[1] = static_cast<double>(Y);

		// compute representation's azimuth, elevation
		self->WidgetRep->WidgetInteraction(e);

		self->EventCallbackCommand->AbortFlagOn();
		self->InvokeEvent(vtkCommand::InteractionEvent);
	}
	if (self->WidgetState != WidgetStateType::Inactive) {
		self->Render();
	}
}

void ToolBar::QVTKToolBar::SquareResize() {
	if (this->DefaultRenderer == nullptr) {
		return;
	}
	auto renWin = this->DefaultRenderer->GetRenderWindow();
	if (renWin == nullptr) {
		return;
	}
	if (renWin->GetNeverRendered()) {
		return;
	}

	auto rep = ToolBar::QVTKToolBarRepresentation::SafeDownCast(this->WidgetRep);
	if (rep == nullptr) {
		return;
	}

	// ToDo
}