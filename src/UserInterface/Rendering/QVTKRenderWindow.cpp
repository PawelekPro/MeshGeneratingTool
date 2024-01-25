#include "QVTKRenderWindow.h"

#include <QLayout>

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

namespace Rendering
{

QVTKRenderWindow::QVTKRenderWindow(QWidget* widget)
    : _widget(widget)
{
    _vtkWidget = new QVTKOpenGLNativeWidget();
    _renderer = vtkSmartPointer<vtkRenderer>::New();
    _vtkWidget->setRenderWindow(vtkGenericOpenGLRenderWindow::New());
    _vtkWidget->renderWindow()->AddRenderer(_renderer);
    _vtkWidget->setFocusPolicy(Qt::StrongFocus);
    generateCoordinateSystemAxes();

    // Background color
    vtkNew<vtkNamedColors> colors;
    _renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

    _renderer->ResetCamera();
    _widget->layout()->addWidget(_vtkWidget);
}

QVTKRenderWindow::~QVTKRenderWindow() { }

void QVTKRenderWindow::generateCoordinateSystemAxes()
{
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    _vtkAxesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();

    vtkSmartPointer<vtkProperty> shaftProps[] = {
        axes->GetXAxisShaftProperty(), axes->GetYAxisShaftProperty(),
        axes->GetZAxisShaftProperty()
    };

    vtkSmartPointer<vtkProperty> tipProps[] = {
        axes->GetXAxisTipProperty(), axes->GetYAxisTipProperty(),
        axes->GetZAxisTipProperty()
    };

    vtkSmartPointer<vtkTextProperty> labelProps[] = {
        axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty(),
        axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty(),
        axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()
    };

    for (int i = 0; i < 3; ++i)
    {
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

} // namespace Rendering
