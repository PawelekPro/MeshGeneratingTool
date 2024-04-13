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
#include "QVTKInteractorStyle.h"
#include "QVTKNavigationWidget.h"

#include <QLayout>

//----------------------------------------------------------------------------
Rendering::QVTKRenderWindow::QVTKRenderWindow(QWidget* widget)
	: _widget(widget) {

	_vtkWidget = new QVTKOpenGLNativeWidget();
	_vtkWidget->setRenderWindow(vtkGenericOpenGLRenderWindow::New());

	_rendererWindow = _vtkWidget->renderWindow();

	// Create renderers and add them to render window
	this->initializeRenderers();

	_interactor = _rendererWindow->GetInteractor();

	_vtkWidget->setFocusPolicy(Qt::StrongFocus);
	this->generateCoordinateSystemAxes();
	this->setWaterMark();

	_interactor->SetRenderWindow(_rendererWindow);

	_camOrientManipulator->SetParentRenderer(this->mRenderers.at(0));
	_camOrientManipulator->SetAnimate(true);
	_camOrientManipulator->AnimateOn();
	vtkSmartPointer<Interactor::QVTKInteractorStyle>
		interactorStyle
		= vtkSmartPointer<Interactor::QVTKInteractorStyle>::New();
	interactorStyle->Initialize(this);
	this->setInteractorStyle(interactorStyle);

	this->RenderScene();
	_widget->layout()->addWidget(_vtkWidget);
}

//----------------------------------------------------------------------------
Rendering::QVTKRenderWindow::~QVTKRenderWindow() {
	for (size_t i = 0; i < static_cast<size_t>(Renderers::Count); i++) {
		this->mRenderers.at(i)->Delete();
	}

	_vtkWidget->deleteLater();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::initializeRenderers() {
	// Background color
	vtkNew<vtkNamedColors> colors;
	this->_rendererWindow->SetNumberOfLayers(static_cast<int>(Layers::Count));
	for (size_t i = 0; i < static_cast<size_t>(Renderers::Count); i++) {
		// Create new renderers
		this->mRenderers.at(i) = vtkSmartPointer<vtkRenderer>::New();
		this->mRenderers.at(i)->SetLayer(static_cast<int>(Layers::Bottom));
		this->mRenderers.at(i)->SetBackground(colors->GetColor3d("SlateGray").GetData()
		);
		this->_rendererWindow->AddRenderer(this->mRenderers.at(i));
		if (i != static_cast<int>(Renderers::Main)) {
			this->mRenderers.at(i)->SetActiveCamera(
				this->mRenderers.at(static_cast<int>(Renderers::Main))->GetActiveCamera());
			this->mRenderers.at(i)->SetInteractive(false);
		}
	}
	this->setActiveRenderer(Renderers::Main);
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::RenderScene() {
	for (size_t i = 0; i < static_cast<size_t>(Renderers::Count); i++) {
		this->mRenderers.at(i)->Render();
	}
	_rendererWindow->Render();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::setInteractorStyle(vtkInteractorStyle* interactorStyle) {
	this->_interactor->SetInteractorStyle(interactorStyle);
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::fitView() {
	this->activeRenderer->ResetCamera();
	this->_rendererWindow->Render();
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
vtkSmartPointer<vtkRenderer> Rendering::QVTKRenderWindow::getActiveRenderer() {
	return this->activeRenderer;
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::enableCameraOrientationWidget() {
	this->_camOrientManipulator->On();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::setActiveRenderer(Rendering::Renderers rendererId) {
	if (this->activeRenderer){
	this->activeRenderer->SetInteractive(false);
	this->activeRenderer->SetLayer(static_cast<int>(Rendering::Layers::Bottom));
	}
	this->activeRenderer = this->mRenderers.at(static_cast<int>(rendererId));
	this->activeRenderer->SetInteractive(true);
	this->activeRenderer->SetLayer(static_cast<int>(Rendering::Layers::Top));
	this->activeRendererId = rendererId;
	this->RenderScene();
}


//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::enableWaterMark() {
	this->_logoWidget->On();
}

//----------------------------------------------------------------------------
void Rendering::QVTKRenderWindow::setWaterMark() {
	QPixmap pixmap(":/uxSetup/icons/watermark.png");

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


void Rendering::QVTKRenderWindow::updateGeometryActors(const GeometryCore::Geometry& geometry){

	this->mRenderers.at(static_cast<int>(Renderers::Main))->Clear();
	this->mRenderers.at(static_cast<int>(Renderers::Faces))->Clear();
	this->mRenderers.at(static_cast<int>(Renderers::Edges))->Clear();

	const GeometryCore::ActorsMap parts = geometry.getPartsActorMap();
	const GeometryCore::ActorsMap faces = geometry.getFacesActorMap();
	const GeometryCore::ActorsMap edges = geometry.getEdgesActorMap();

    for(const auto& actor : parts) {
        this->mRenderers.at(static_cast<int>(Renderers::Main))->AddActor(actor.second);
    }
    for(const auto& actor : faces) {
        this->mRenderers.at(static_cast<int>(Renderers::Faces))->AddActor(actor.second);
    }
    for(const auto& actor : edges) {
		actor.second->GetProperty()->SetColor(0.0, 1.0, 0.0);
        this->mRenderers.at(static_cast<int>(Renderers::Edges))->AddActor(actor.second);
    }
	this->setActiveRenderer(Renderers::Main);
	this->RenderScene();
}