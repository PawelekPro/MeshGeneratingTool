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

#include <vtkAbstractWidget.h>
#include <vtkActor.h>
#include <vtkBoundingBox.h>
#include <vtkButtonWidget.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkEvent.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkWeakPointer.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetRepresentation.h>

namespace ToolBar {

template <typename EnumT>
constexpr typename std::underlying_type<EnumT>::type to_underlying(const EnumT& e) noexcept {
	return static_cast<typename std::underlying_type<EnumT>::type>(e);
}

class QVTKToolBarRepresentation : public vtkWidgetRepresentation {
public:
	static QVTKToolBarRepresentation* New();
	vtkTypeMacro(QVTKToolBarRepresentation, vtkWidgetRepresentation);

	enum class InteractionStateType : int {
		Outside = 0, // corresponds to vtkCameraOrientationWidget::Inactive
		Hovering = 1, // corresponds to vtkCameraOrientationWidget::Hot
		Rotating = 2 // corresponds to vtkCameraOrientationWidget::Active
	};

	enum class AnchorType : int {
		LowerLeft = 0,
		UpperLeft,
		LowerRight,
		UpperRight
	};

	InteractionStateType GetInteractionStateAsEnum() noexcept {
		// clamp to 0-2
		this->InteractionState = this->InteractionState < 0 ? 0 : (this->InteractionState > 2 ? 2 : this->InteractionState);
		// convert
		return static_cast<InteractionStateType>(this->InteractionState);
	}

	/**
	 * Get/Set the widget anchor type
	 */
	AnchorType GetAnchorPosition() { return this->AnchorPosition; }
	void AnchorToLowerLeft() {
		this->AnchorPosition = AnchorType::LowerLeft;
		this->Modified();
	}
	void AnchorToUpperLeft() {
		this->AnchorPosition = AnchorType::UpperLeft;
		this->Modified();
	}
	void AnchorToLowerRight() {
		this->AnchorPosition = AnchorType::LowerRight;
		this->Modified();
	}
	void AnchorToUpperRight() {
		this->AnchorPosition = AnchorType::UpperRight;
		this->Modified();
	}

	/**
	 * The interaction state may be set from a widget (e.g., vtkCameraOrientationWidget) or
	 * other object. This call updates the representation to match the interaction state.
	 */
	void ApplyInteractionState(const InteractionStateType& state);
	void ApplyInteractionState(const int& state);

	/**
	 * These are methods that satisfy vtkWidgetRepresentation's API.
	 */
	void PlaceWidget(double*) override { }
	void BuildRepresentation() override;
	void StartWidgetInteraction(double eventPos[2]) override;
	void WidgetInteraction(double newEventPos[2]) override;
	void EndWidgetInteraction(double newEventPos[2]) override;
	int ComputeInteractionState(int X, int Y, int modify = 0) override;
	double* GetBounds() VTK_SIZEHINT(6) override;

	/**
	 * Methods supporting, and required by, the rendering process.
	 */
	void ReleaseGraphicsResources(vtkWindow*) override;
	int RenderOpaqueGeometry(vtkViewport*) override;
	int RenderTranslucentPolygonalGeometry(vtkViewport*) override;
	vtkTypeBool HasTranslucentPolygonalGeometry() override;

	/**
	 * Shallow copy of an axes actor. Overloads the virtual vtkProp method.
	 */
	void ShallowCopy(vtkProp* prop) override;

	double* GetSize() { return this->Size; }

	double* GetPadding() { return this->Padding; }

	void createDefaultGeometry(vtkRenderWindowInteractor* iren);
	void enableButtons() {
		this->buttonWidget->On();
	}

protected:
	QVTKToolBarRepresentation();
	~QVTKToolBarRepresentation() override;

private:
	void addButton(vtkRenderWindowInteractor* iren);
	void createDefaultProperties();

	vtkRenderWindowInteractor* _interactor;
	vtkNew<vtkActor> container;
	vtkNew<vtkPlaneSource> containerSource;
	double Bounds[6] = {};
	// Event tracking
	double LastEventPosition[3] = {};

	// Positioning of the representation within a parent renderer.
	AnchorType AnchorPosition = AnchorType::UpperLeft;

	double Padding[2] = { 0.8, 0.05 }; // In display coords.
	double Size[2] = { 1, 1 }; // In display coords.

	vtkSmartPointer<vtkTexturedButtonRepresentation2D> buttonRepresentation;
	vtkSmartPointer<vtkButtonWidget> buttonWidget;
};

class QVTKToolBar : public vtkAbstractWidget {
public:
	static QVTKToolBar* New();
	vtkTypeMacro(QVTKToolBar, vtkAbstractWidget);

	void CreateDefaultRepresentation() override;
	void SetParentRenderer(vtkRenderer* renderer);

	/**
	 * Fits the widget's renderer to a square viewport.
	 */
	void SquareResize();

	void createDefaultGeometry() {
		auto rep = ToolBar::QVTKToolBarRepresentation::SafeDownCast(this->WidgetRep);
		if (rep == nullptr) {
			std::cout << "SIEMA" << std::endl;
			return;
		} else {
			rep->createDefaultGeometry(this->Interactor);
		}
	}

	void enableButtons() {
		auto rep = ToolBar::QVTKToolBarRepresentation::SafeDownCast(this->WidgetRep);
		if (rep == nullptr) {
			std::cout << "SIEMA" << std::endl;
			return;
		} else {
			rep->enableButtons();
		}
	}

protected:
	QVTKToolBar();
	~QVTKToolBar() override = default;

	// These methods handle events
	void ComputeWidgetState(int X, int Y, int modify = 0);
	static void SelectAction(vtkAbstractWidget*);
	static void EndSelectAction(vtkAbstractWidget*);
	static void MoveAction(vtkAbstractWidget*);

	// This one control the representation
	void ToolBarWidgetRepresentation();

	// Manage the state of the widget
	enum class WidgetStateType : int {
		Inactive, // mouse is not over the widget, none of the handles are selected.
		Hot, // mouse is over the widget but none of the handles are selected
		Active // any one handle is selected, representation could be rotating.
	};
	WidgetStateType WidgetState = WidgetStateType::Inactive;

	vtkWeakPointer<vtkRenderer> ParentRenderer;
	int ResizeObserverTag = -1;
};
}
