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

#include "QVTKRenderWindow.h"

#include <QLayout>

//----------------------------------------------------------------------------
Rendering::QVTKRenderWindow::QVTKRenderWindow(QWidget* widget)
	: _widget(widget)
	, _rendererWindow(vtkGenericOpenGLRenderWindow::New())
	, _renderer(vtkSmartPointer<vtkRenderer>::New())
	, _vtkWidget(new QVTKOpenGLNativeWidget())
	, _shapePicker(vtkSmartPointer<IVtkTools_ShapePicker>::New())
	, _interactorStyle(vtkSmartPointer<QVTKInteractorStyle>::New()) {

	_vtkWidget->setRenderWindow(_rendererWindow);
	_interactor = _vtkWidget->interactor();

	vtkNew<vtkNamedColors> colors;
	_renderer->GetActiveCamera()->ParallelProjectionOff();
	_renderer->LightFollowCameraOn();
	_rendererWindow->AddRenderer(_renderer);
	_renderer->SetBackground(colors->GetColor3d("LightSlateGray").GetData());

	// _vtkWidget->setFocusPolicy(Qt::StrongFocus);

	this->generateCoordinateSystemAxes();
	this->setWaterMark();

	_camOrientManipulator->SetParentRenderer(_renderer);
	_camOrientManipulator->SetAnimate(true);
	_camOrientManipulator->AnimateOn();

	// Setup IVtk Shape picker
	_shapePicker->SetRenderer(_renderer);
	_shapePicker->SetTolerance(0.025);

	// Setup interactor style
	_interactorStyle->setRenderer(_renderer);
	_interactorStyle->setQVTKRenderWindow(this);
	_interactorStyle->setPicker(_shapePicker);

	_interactor->SetInteractorStyle(_interactorStyle);

	_renderer->ResetCamera();
	_rendererWindow->Render();
	_widget->layout()->addWidget(_vtkWidget);
}

//----------------------------------------------------------------------------
Rendering::QVTKRenderWindow::~QVTKRenderWindow() {

	if (_renderer)
		_renderer->Delete();

	if (_vtkWidget)
		_vtkWidget->deleteLater();

	if (_shapePicker)
		_shapePicker->Delete();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::addActor(vtkActor* actor) {
	_renderer->AddActor(actor);
}
//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::RenderScene() {
	_renderer->Render();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::fitView() const {
	_renderer->ResetCamera();
	_rendererWindow->Render();
}
//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::generateCoordinateSystemAxes() {
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	this->_vtkAxesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();

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

	this->_vtkAxesWidget->SetOrientationMarker(axes);
	this->_vtkAxesWidget->SetInteractor(this->_interactor);
	this->_vtkAxesWidget->SetViewport(0.0, 0.0, 0.15, 0.25);
	this->_vtkAxesWidget->SetEnabled(true);
	this->_vtkAxesWidget->InteractiveOff();
};

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::enableCameraOrientationWidget() {
	this->_camOrientManipulator->On();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::enableWaterMark() {
	this->_logoWidget->On();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::clearRenderer() {
	this->_renderer->Clear();
	this->_renderer->RemoveActor(this->model->getMeshActor());

	NCollection_List<Handle(QIVtkSelectionPipeline)> pipelinesList
		= _interactorStyle->getPipelines();
	NCollection_List<Handle(QIVtkSelectionPipeline)>::Iterator pIt(pipelinesList);
	for (; pIt.More(); pIt.Next()) {
		const Handle(QIVtkSelectionPipeline)& pipeline = pIt.Value();
		pipeline->RemoveFromRenderer(_renderer);
	}
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::setWaterMark() {
	QPixmap pixmap(":/uiSetup/icons/watermark.png");

	if (pixmap.isNull()) {
		qDebug() << "Failed to load image from resource";
		vtkLogF(ERROR, "Failed to load image from resource");
	}

	vtkNew<vtkQImageToImageSource> qimageToImageSource;
	QImage qimage = pixmap.toImage();

	qimageToImageSource->SetQImage(&qimage);
	qimageToImageSource->Update();

	vtkNew<vtkLogoRepresentation> logoRepresentation;
	logoRepresentation->SetPosition(0.01, 0.6);
	logoRepresentation->SetPosition2(0.15, 0.65);
	logoRepresentation->SetImage(qimageToImageSource->GetOutput());

	this->_logoWidget = vtkSmartPointer<vtkLogoWidget>::New();
	this->_logoWidget->SetInteractor(this->_interactor);
	this->_logoWidget->SetRepresentation(logoRepresentation);
	this->_logoWidget->ProcessEventsOff();
	this->_logoWidget->ManagesCursorOff();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::addShapeToRenderer(const TopoDS_Shape& shape) {

	static Standard_Integer ShapeID
		= _interactorStyle->getPipelinesMapSize();
	++ShapeID;

	Handle(QIVtkSelectionPipeline) pipeline = new QIVtkSelectionPipeline(shape, ShapeID);
	pipeline->AddToRenderer(_renderer);

	_interactorStyle->addPipeline(pipeline, ShapeID);
	fitView();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::addPipelinesToRenderer() {
	NCollection_List<Handle(QIVtkSelectionPipeline)> pipelinesList
		= _interactorStyle->getPipelines();
	NCollection_List<Handle(QIVtkSelectionPipeline)>::Iterator pIt(pipelinesList);
	for (; pIt.More(); pIt.Next()) {
		const Handle(QIVtkSelectionPipeline)& pipeline = pIt.Value();
		pipeline->AddToRenderer(_renderer);
	}
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::setSelectionMode(IVtk_SelectionMode mode) {
	_interactorStyle->setSelectionMode(mode);
}