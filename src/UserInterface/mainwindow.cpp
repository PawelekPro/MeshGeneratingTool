#include "mainwindow.h"
#include "../CADManager/STEPFileReader.h"
#include "./ui_mainwindow.h"

#include <vtkAxesActor.h>

#include <vtkCaptionActor2D.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkVersion.h>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	ui->setupUi(this);

	// Set initial sizes of the splitter sections
	QList<int> sizes;
	sizes << 100 << 400;
	ui->mainSplitter->setSizes(sizes);

	QVTKRender = new Rendering::QVTKRenderWindow(ui->modelView);
	Importing::STEPFileReader stepReader {};
	stepReader.load("/home/pgilewicz/geometrySample/rubixCube.step");
	Importing::ActorsMap actorsMap = stepReader.getVTKActorsMap();
	QVTKRender->addActors(actorsMap);
}

MainWindow::~MainWindow() {
	delete QVTKRender;
	delete ui;
}
