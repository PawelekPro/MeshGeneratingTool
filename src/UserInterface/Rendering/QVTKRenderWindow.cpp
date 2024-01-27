#include "QVTKRenderWindow.h"

#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCaptionActor2D.h>
#include <vtkNamedColors.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPlatonicSolidSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>

#include <QLayout>

Rendering::QVTKRenderWindow::QVTKRenderWindow(QWidget* widget)
	: _widget(widget) {
	_vtkWidget = new QVTKOpenGLNativeWidget();
	_renderer = vtkSmartPointer<vtkRenderer>::New();
	_vtkWidget->setRenderWindow(vtkGenericOpenGLRenderWindow::New());

	_rendererWindow = _vtkWidget->renderWindow();
	_rendererWindow->AddRenderer(_renderer);

	_vtkWidget->setFocusPolicy(Qt::StrongFocus);
	generateCoordinateSystemAxes();

	// Background color
	vtkNew<vtkNamedColors> colors;
	_renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

	_renderer->ResetCamera();
	_widget->layout()->addWidget(_vtkWidget);
}

Rendering::QVTKRenderWindow::~QVTKRenderWindow() { }

void Rendering::QVTKRenderWindow::addActors(const Importing::ActorsMap& actorsMap) {
	for (const auto& entry : actorsMap) {
		vtkSmartPointer<vtkActor> actor = entry.second;
		this->_renderer->AddActor(actor);
	}
	this->_renderer->ResetCamera();
	this->_rendererWindow->Render();
}

void Rendering::QVTKRenderWindow::generateCoordinateSystemAxes() {
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	_vtkAxesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();

	vtkSmartPointer<vtkProperty> shaftProps[] = { axes->GetXAxisShaftProperty(),
		axes->GetYAxisShaftProperty(),
		axes->GetZAxisShaftProperty() };

	vtkSmartPointer<vtkProperty> tipProps[] = { axes->GetXAxisTipProperty(),
		axes->GetYAxisTipProperty(),
		axes->GetZAxisTipProperty() };

	vtkSmartPointer<vtkTextProperty> labelProps[] = {
		axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty(),
		axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty(),
		axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()
	};

	for (int i = 0; i < 3; ++i) {
		shaftProps[i]->SetColor(1.0, 1.0, 1.0);
		tipProps[i]->SetColor(1.0, 1.0, 1.0);
		labelProps[i]->ShadowOff();
		labelProps[i]->ItalicOff();
	}

	_vtkAxesWidget->SetOrientationMarker(axes);
	_vtkAxesWidget->SetInteractor(_vtkWidget->renderWindow()->GetInteractor());
	_vtkAxesWidget->SetViewport(0.0, 0.0, 0.15, 0.25);
	_vtkAxesWidget->SetEnabled(1);
	_vtkAxesWidget->InteractiveOn();
};
