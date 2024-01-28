#ifndef QVTKRENDERWINDOW_H
#define QVTKRENDERWINDOW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include "../../CADManager/STEPFileReader.h"
#include <vtkInteractorStyle.h>

namespace Rendering {

class QVTKRenderWindow {
public:
	QVTKRenderWindow(QWidget* widget);
	~QVTKRenderWindow();

	void generateCoordinateSystemAxes();
	void addActors(const Importing::ActorsMap& actorsMap);

	void setInteractorStyle(vtkInteractorStyle* interactorStyle);
	void fitView();

private:
	QWidget* _widget;

	// The Qt widget containing a VTK viewport
	QVTKOpenGLNativeWidget* _vtkWidget;

	// The VTK renderer (add VTK actors to it to build the scene)
	vtkSmartPointer<vtkRenderer> _renderer;

	// The VTK renderer window
	vtkSmartPointer<vtkRenderWindow> _rendererWindow;

	// The VTK render window interactor
	vtkSmartPointer<vtkRenderWindowInteractor> _interactor;

	vtkSmartPointer<vtkOrientationMarkerWidget> _vtkAxesWidget;
};
}; // namespace Rendering

#endif