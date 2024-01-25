#ifndef QVTKRENDERWINDOW_H
#define QVTKRENDERWINDOW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

namespace Rendering
{

class QVTKRenderWindow
{
  public:
    QVTKRenderWindow(QWidget *widget);
    ~QVTKRenderWindow();

    void generateCoordinateSystemAxes();

  private:
    QWidget *_widget;

    // the Qt widget containing a VTK viewport
    QVTKOpenGLNativeWidget *_vtkWidget;

    // the VTK renderer (add VTK actors to it to build the scene).
    vtkSmartPointer<vtkRenderer> _renderer;

    vtkSmartPointer<vtkOrientationMarkerWidget> _vtkAxesWidget;
};
}; // namespace Rendering

#endif