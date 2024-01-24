#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <vtkAxesActor.h>

#include <vtkCaptionActor2D.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkVersion.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVTKRender = new Rendering::QVTKRenderWindow(ui->qvtkWidget);
}

MainWindow::~MainWindow()
{
    delete QVTKRender;
    delete ui;
}
