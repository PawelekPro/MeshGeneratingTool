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

*=============================================================================
* File      : QQVTKCameraOrientationWidget.h
* Author    : Paweł Gilewicz
* Date      : 06/01/2025
*/
#ifndef QQVTKCameraOrientationWidget_H
#define QQVTKCameraOrientationWidget_H

#include <vtkAbstractWidget.h>
#include <vtkInteractionWidgetsModule.h> // needed for export macro
#include <vtkWeakPointer.h> // for weak pointer ivar
#include <vtkWidgetRepresentation.h>
#include <vtkWrappingHints.h> // For VTK_MARSHALAUTO

VTK_ABI_NAMESPACE_BEGIN

class vtkActor;
class vtkDiskSource;
class vtkDoubleArray;
class vtkEllipticalButtonSource;
class vtkImageData;
class vtkPoints;
class vtkPolyData;
class vtkPropCollection;
class vtkProperty;
class vtkPropPicker;
class vtkTextProperty;
class vtkTexture;
class vtkTubeFilter;
class vtkCameraInterpolator;
class vtkRenderer;

#ifdef WIN32
#define ORIENTATION_REPRESENTATION_EXPORT __declspec(dllexport)
#else
#define ORIENTATION_REPRESENTATION_EXPORT
#endif

class ORIENTATION_REPRESENTATION_EXPORT QVTKCameraOrientationRepresentation
	: public vtkWidgetRepresentation {
public:
	static QVTKCameraOrientationRepresentation* New();
	vtkTypeMacro(QVTKCameraOrientationRepresentation, vtkWidgetRepresentation);
	void PrintSelf(ostream& os, vtkIndent indent) override;

	enum class InteractionStateType : int {
		Outside = 0, // corresponds to vtkCameraOrientationWidget::Inactive
		Hovering, // corresponds to vtkCameraOrientationWidget::Hot
		Rotating // corresponds to vtkCameraOrientationWidget::Active
	};

	/**
	 * The interaction state may be set from a widget (e.g., vtkCameraOrientationWidget) or
	 * other object. This call updates the representation to match the interaction state.
	 */
	void ApplyInteractionState(const InteractionStateType& state);
	void ApplyInteractionState(const int& state);

	/**
	 * Convenient method to get InteractionState as enum.
	 * This method clamps the interaction state to possible values.
	 * Hence, it does not raise any exceptions.
	 */
	InteractionStateType GetInteractionStateAsEnum() noexcept {
		// clamp to 0-2
		this->InteractionState = this->InteractionState < 0 ? 0 : (this->InteractionState > 2 ? 2 : this->InteractionState);
		// convert
		return static_cast<InteractionStateType>(this->InteractionState);
	}

	///@{
	/**
	 * Get/Set the widget size in display coordinates.
	 */
	vtkSetVector2Macro(Size, int);
	vtkGetVector2Macro(Size, int);
	///@}

	///@{
	/**
	 * Get/Set the widget padding in display coordinates.
	 */
	vtkSetVector2Macro(Padding, int);
	vtkGetVector2Macro(Padding, int);
	///@}

	enum class AnchorType : int {
		LowerLeft = 0,
		UpperLeft,
		LowerRight,
		UpperRight
	};

	///@{
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
	///@}

	///@{
	/**
	 * Set the total length of the axes in 3 dimensions.
	 * This is basis of normalization. Default value: 1.
	 */
	vtkSetMacro(TotalLength, double);
	vtkGetMacro(TotalLength, double);
	///@}

	///@{
	/**
	 * Set the normalized (0-1) diameter of the Handle.
	 * Default value: 0.4
	 */
	vtkSetMacro(NormalizedHandleDia, double);
	vtkGetMacro(NormalizedHandleDia, double);
	///@}

	///@{
	/**
	 * Orientation properties. (read only)
	 */
	vtkGetMacro(Azimuth, double);
	vtkGetVector3Macro(Back, double);
	vtkGetMacro(Elevation, double);
	vtkGetVector3Macro(Up, double);
	///@}

	///@{
	/**
	 * Set shaft's resolution.
	 */
	vtkSetClampMacro(ShaftResolution, int, 3, 256);
	vtkGetMacro(ShaftResolution, int);
	///@}

	///@{
	/**
	 * Set Handle's circumferential resolution.
	 */
	vtkSetClampMacro(HandleCircumferentialResolution, int, 3, 256);
	vtkGetMacro(HandleCircumferentialResolution, int);
	///@}

	///@{
	/**
	 * Set container's circumferential resolution.
	 */
	vtkSetClampMacro(ContainerCircumferentialResolution, int, 3, 256);
	vtkGetMacro(ContainerCircumferentialResolution, int);
	///@}

	///@{
	/**
	 * Set container's radial resolution.
	 */
	vtkSetClampMacro(ContainerRadialResolution, int, 3, 256);
	vtkGetMacro(ContainerRadialResolution, int);
	///@}

	///@{
	/**
	 * Get picked axis, direction
	 */
	vtkGetMacro(PickedAxis, int);
	vtkGetMacro(PickedDir, int);
	///@}

	///@{
	/**
	 * Get the '+' axis label properties.
	 */
	vtkTextProperty* GetXPlusLabelProperty();
	vtkTextProperty* GetYPlusLabelProperty();
	vtkTextProperty* GetZPlusLabelProperty();
	///@}

	///@{
	/**
	 * Get the '-' axis label properties.
	 */
	vtkTextProperty* GetXMinusLabelProperty();
	vtkTextProperty* GetYMinusLabelProperty();
	vtkTextProperty* GetZMinusLabelProperty();
	///@}

	/**
	 * Get the container property.
	 */
	vtkProperty* GetContainerProperty();

	///@{
	/**
	 * Show container to indicate mouse presence.
	 */
	void SetContainerVisibility(bool state);
	vtkBooleanMacro(ContainerVisibility, bool);
	bool GetContainerVisibility();
	///@}

	/**
	 * For some exporters and other other operations we must be
	 * able to collect all the actors or volumes. These methods
	 * are used in that process.
	 */
	void GetActors(vtkPropCollection*) override;

	/**
	 * Retrieve internal transform of this widget representation.
	 */
	vtkTransform* GetTransform();

	///@{
	/**
	 * These are methods that satisfy vtkWidgetRepresentation's API.
	 */
	void PlaceWidget(double*) override { } // this representation is an overlay. Doesn't need this.
	void BuildRepresentation() override;
	void StartWidgetInteraction(double eventPos[2]) override;
	void WidgetInteraction(double newEventPos[2]) override;
	void EndWidgetInteraction(double newEventPos[2]) override;
	int ComputeInteractionState(int X, int Y, int modify = 0) override;
	double* GetBounds() VTK_SIZEHINT(6) override;
	///@}

	///@{
	/**
	 * Methods supporting, and required by, the rendering process.
	 */
	void ReleaseGraphicsResources(vtkWindow*) override;
	int RenderOpaqueGeometry(vtkViewport*) override;
	int RenderTranslucentPolygonalGeometry(vtkViewport*) override;
	vtkTypeBool HasTranslucentPolygonalGeometry() override;
	///@}

	/**
	 * Shallow copy of an axes actor. Overloads the virtual vtkProp method.
	 */
	void ShallowCopy(vtkProp* prop) override;

	/**
	 * Is a grabber button picked.
	 */
	bool IsAnyHandleSelected() { return (this->PickedAxis != -1) && (this->PickedDir != -1); }

protected:
	QVTKCameraOrientationRepresentation();
	~QVTKCameraOrientationRepresentation() override;

	virtual void CreateDefaultGeometry();
	virtual void CreateDefaultProperties();
	virtual void PositionHandles();
	virtual void HighlightHandle();
	virtual void Rotate(double newEventPos[2]);
	void RegisterPickers() override;
	void FinalizeHandlePicks();

	// description of source shapes.
	vtkNew<vtkDiskSource> ContainerSource;
	vtkNew<vtkEllipticalButtonSource> HandleSources[3][2];
	vtkNew<vtkTubeFilter> ShaftGlyphs;

	// geometries of handles and shafts. (position, color info)
	vtkNew<vtkPolyData> Skeleton;
	vtkNew<vtkPoints> Points; // used to store handle positions, also used by shafts

	// defaults are slight variations of r, y, g
	vtkNew<vtkDoubleArray> AxesColors;

	// props
	vtkNew<vtkActor> Container;
	vtkNew<vtkActor> Handles[3][2];
	vtkNew<vtkActor> Shafts;

	// font-sz, font-type, frame color of the labels.
	vtkNew<vtkTextProperty> AxisVectorTextProperties[3][2];
	vtkNew<vtkImageData> LabelImages[3][2];
	vtkNew<vtkTexture> LabelTextures[3][2];

	vtkNew<vtkPropPicker> HandlePicker;

	// Store rotation of gizmo.
	vtkNew<vtkTransform> Transform;

	// Positioning of the representation within a parent renderer.
	AnchorType AnchorPosition = AnchorType::UpperRight;
	int Padding[2] = { 10, 10 }; // In display coords.
	int Size[2] = { 120, 120 }; // In display coords.

	// Geometrical, textual, interaction description of the representation.
	const char* AxisLabelsText[3][2] = { { "x", "-x" }, { "y", "-y" }, { "z", "-z" } };
	double Azimuth = 0.;
	double Back[3] = { 0., 0., -1. };
	double Bounds[6] = {};
	double Elevation = 0.;
	double MotionFactor = 1.;
	double NormalizedHandleDia = 0.4;
	double TotalLength = 1.;
	double Up[3] = { 0., 1., 0. };
	int ContainerCircumferentialResolution = 32;
	int ContainerRadialResolution = 1;
	int HandleCircumferentialResolution = 32;
	int ShaftResolution = 10;

	// Picking information.
	int PickedAxis = -1;
	int LastPickedAx = -1;
	int PickedDir = -1;
	int LastPickedDir = -1;

	// Event tracking
	double LastEventPosition[3] = {};

private:
	QVTKCameraOrientationRepresentation(const QVTKCameraOrientationRepresentation&) = delete;
	void operator=(const QVTKCameraOrientationRepresentation&) = delete;
};

class ORIENTATION_REPRESENTATION_EXPORT QVTKCameraOrientationWidget
	: public vtkAbstractWidget {
public:
	static QVTKCameraOrientationWidget* New();
	vtkTypeMacro(QVTKCameraOrientationWidget, vtkAbstractWidget);
	void PrintSelf(ostream& os, vtkIndent indent) override;

	///@{
	/**
	 * Enable jump-to-axis-view animation.
	 * See AnimatorTotalFrames.
	 */
	vtkSetMacro(Animate, bool);
	vtkGetMacro(Animate, bool);
	vtkBooleanMacro(Animate, bool);
	///@}

	///@{
	/**
	 * Length of animation. (in frames)
	 */
	vtkSetClampMacro(AnimatorTotalFrames, int, 2, VTK_INT_MAX);
	vtkGetMacro(AnimatorTotalFrames, int);
	///@}

	/**
	 * Create a QVTKCameraOrientationRepresentation.
	 */
	void CreateDefaultRepresentation() override;

	/**
	 * Fits the widget's renderer to a square viewport.
	 */
	void SquareResize();

	///@{
	/**
	 * This widget shows and manipulates the orientation of
	 * the parent renderer's active camera.
	 *
	 * Note: The renderer must be part of a render window for
	 * the widget to appear.
	 */
	void SetParentRenderer(vtkRenderer* renderer);
	vtkRenderer* GetParentRenderer();
	///@}

protected:
	QVTKCameraOrientationWidget();
	~QVTKCameraOrientationWidget() override;

	// These methods handle events
	void ComputeWidgetState(int X, int Y, int modify = 0);
	static void SelectAction(vtkAbstractWidget*);
	static void EndSelectAction(vtkAbstractWidget*);
	static void MoveAction(vtkAbstractWidget*);

	// These control the representation and parent renderer's camera.
	void OrientParentCamera(double back[3], double up[3]);
	void OrientWidgetRepresentation();
	void InterpolateCamera(int t);

	// Manage the state of the widget
	enum class WidgetStateType : int {
		Inactive, // mouse is not over the widget, none of the handles are selected.
		Hot, // mouse is over the widget but none of the handles are selected
		Active // any one handle is selected, representation could be rotating.
	};
	WidgetStateType WidgetState = WidgetStateType::Inactive;

	vtkWeakPointer<vtkRenderer> ParentRenderer;

	// Store camera interpolations.
	vtkNew<vtkCameraInterpolator> CameraInterpolator;

	bool Animate = true;
	int AnimatorTotalFrames = 20;

	int ResizeObserverTag = -1;

private:
	QVTKCameraOrientationWidget(const QVTKCameraOrientationWidget&) = delete;
	void operator=(const QVTKCameraOrientationWidget&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif