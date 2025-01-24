/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

#include "GeometryActionsHandler.hpp"
#include "MeshActionsHandler.hpp"

#include "ProgressBarPlugin.hpp"

//----------------------------------------------------------------------------
MainWindow::MainWindow(std::shared_ptr<ModelInterface> aModelInterface, QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, _modelInterface(aModelInterface) {

	ui->setupUi(this);
	// Set initial sizes of the splitter sections
	QList<int> sizes;
	sizes << 100 << 400;
	ui->mainSplitter->setSizes(sizes);
	ui->propertiesSplitter->setSizes(sizes);

	this->QVTKRender = new Rendering::QVTKRenderWindow(ui->modelView);
	this->QVTKRender->enableCameraOrientationWidget();
	this->QVTKRender->enableWaterMark();
	this->ui->ribbonBar->initialize();

	this->progressBar = new ProgressBar(this);

	_renderSignalHandler = new Rendering::RenderSignalHandler(QVTKRender, _modelInterface->modelDataView(), this);
	_renderSignalSender = new RenderSignalSender(this);
	_modelHandler = new ModelActionsHandler(_modelInterface, _renderSignalSender, ui->treeWidget, progressBar, this);

	this->ui->statusBar->addWidget(progressBar);

	this->ui->treeWidget->setModelHandler(_modelHandler);

	this->connectActionsToModel();
	this->connectModelToRenderWindow(_renderSignalSender, _renderSignalHandler);

	AppTheme& appTheme = AppTheme::getInstance();
	appTheme.initializeAppStylesheet();
}
//----------------------------------------------------------------------------
MainWindow::~MainWindow() {
	QObject::disconnect(this->ui->treeWidget, &QTreeWidget::itemSelectionChanged,
		this, &MainWindow::onItemSelectionChanged);

	delete _modelHandler;
	delete QVTKRender;
	delete progressBar;
	delete ui;
}

//----------------------------------------------------------------------------
void MainWindow::connectActionsToModel() {

	connect(ui->actionUndo, &QAction::triggered,
		_modelHandler, &ModelActionsHandler::undo);

	connect(ui->actionImportSTEP, &QAction::triggered,
		_modelHandler->_geometryHandler, &GeometryActionsHandler::importSTEP);

	connect(ui->actionImportSTL, &QAction::triggered,
		_modelHandler->_geometryHandler, &GeometryActionsHandler::importSTL);

	connect(&this->buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
		this, &MainWindow::handleSelectorButtonClicked);

	connect(ui->actionGenerateMesh, &QAction::triggered, [this]() {
		_modelHandler->_meshHandler->meshSurface();
		ui->actionShowMesh->setChecked(true);
	});

	connect(this->ui->treeWidget, &QTreeWidget::itemSelectionChanged,
		this, &MainWindow::onItemSelectionChanged, Qt::DirectConnection);
}

//----------------------------------------------------------------------------
void MainWindow::connectModelToRenderWindow(RenderSignalSender* aSignalSender, Rendering::RenderSignalHandler* aSignalHandler) {
	// TODO: unify the handler so that both are set in the same way (now one is a field, the other via method)
	// TODO: Fun task - encapsulate the connections in a helper map/class/namespace
	GeometrySignalSender* geometrySignals = aSignalSender->geometrySignals;
	Rendering::GeometryRenderHandler* geoRender = aSignalHandler->geometry();

	MeshSignalSender* meshSignals = aSignalSender->meshSignals;
	Rendering::MeshRenderHandler* meshRender = aSignalHandler->mesh();

	QObject::connect(geometrySignals, &GeometrySignalSender::geometryImported,
		geoRender, &Rendering::GeometryRenderHandler::addAllShapesToRenderer);

	QObject::connect(meshSignals, &MeshSignalSender::meshGenerated,
		meshRender, &Rendering::MeshRenderHandler::showMeshActor);

	QObject::connect(ui->actionShowMesh, &QAction::toggled, [geoRender, meshRender](bool checked) {
		if (checked) {
			meshRender->showMeshActor();
		} else {
			geoRender->showExistingShapes();
		}
	});

	QObject::connect(geometrySignals, &GeometrySignalSender::requestSelectedShapes,
		geoRender, &Rendering::GeometryRenderHandler::selectedShapesRequested);

	QObject::connect(geoRender, &Rendering::GeometryRenderHandler::sendSelectedShapes,
		geometrySignals, &GeometrySignalSender::receiveSelectedShapes);

	QObject::connect(geometrySignals, &GeometrySignalSender::requestSelectionType,
		geoRender, &Rendering::GeometryRenderHandler::selectionTypeRequested);

	QObject::connect(geoRender, &Rendering::GeometryRenderHandler::sendSelctionType,
		geometrySignals, &GeometrySignalSender::receiveSelectionType);
}

//----------------------------------------------------------------------------
void MainWindow::handleSelectorButtonClicked(QAbstractButton* button) {
	for (QAbstractButton* btn : this->buttonGroup.buttons()) {
		if (btn != button)
			btn->setChecked(false);
	}
	// std::cout<<"Button id: " << buttonGroup.id(button) << std::endl;
	// Set active layer according to selected button
	qDebug() << "Button" << buttonGroup.id(button) << "clicked";
	// this->QVTKRender->setActiveRenderer(static_cast<Rendering::Renderers>(
	// 	buttonGroup.id(button)));
}
//----------------------------------------------------------------------------
void MainWindow::onItemSelectionChanged() {
	QList<QTreeWidgetItem*> itemsList = this->ui->treeWidget->selectedItems();

	// Proceed only if there is a selected item
	if (!itemsList.isEmpty()) {
		// Cast the first selected item to TreeItem
		TreeItem* item = dynamic_cast<TreeItem*>(itemsList.takeFirst());

		// Check if the cast was successful and the item is a valid TreeItem
		if (item && item->_propModel) {
			PropertiesModel* model = item->_propModel;

			// Set the properties table model
			this->ui->propertiesTable->setModel(model);

			// Configure the header
			QHeaderView* header = this->ui->propertiesTable->horizontalHeader();
			header->setSectionResizeMode(QHeaderView::ResizeToContents);
			header->setStretchLastSection(true);
			header->setSectionResizeMode(QHeaderView::Interactive);
		} else {
			qDebug() << "Selected item is not a TreeItem or has a null PropertiesModel pointer.";
		}
	}
}

void MainWindow::onShowDialogButtonClicked() {
	PreferencesDialog dialog(this);

	// Show the dialog
	dialog.exec();
}
