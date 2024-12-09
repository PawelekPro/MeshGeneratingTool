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

#include "MainWindow.hpp"

#include "./ui_MainWindow.h"

//----------------------------------------------------------------------------
MainWindow::MainWindow(std::shared_ptr<ModelInterface> aModelInterface, QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, _modelInterface(aModelInterface)
	, _modelCommandManager(new ModelCommandManager()) {
	
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


	_renderSignalHandler = new Rendering::RenderSignalHandler(QVTKRender, _modelInterface->modelDataView(), this);
	_modelHandler = new ModelActionsHandler(_modelInterface, progressBar, _modelCommandManager, this);
	this->connectModelToRenderWindow(_modelHandler, _renderSignalHandler);

	this->progressBar = new ProgressBar(this);
 	this->ui->statusBar->addWidget(progressBar);

	this->ui->treeWidget->setModelHandler(_modelHandler);

	this->setConnections();
	this->initializeActions();

	// this->buttonGroup.addButton(this->ui->volumeSelectorButton,
	// 	static_cast<int>(Rendering::Renderers::Main));

	// this->buttonGroup.addButton(this->ui->facesSelectorButton,
	// 	static_cast<int>(Rendering::Renderers::Faces));

	// this->buttonGroup.addButton(this->ui->edgesSelectorButton,
	// 	static_cast<int>(Rendering::Renderers::Edges));
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
void MainWindow::setConnections() {
	// connect(ui->actionImportSTEP, &QAction::triggered, [this]() {
	// 	FileDialogUtils::executeWithFileSelection([this](const QString fname) { _modelInterface->importSTEP(fname, progressBar);}, "Import", FileDialogUtils::FilterSTEP);
	// });

	// connect(ui->actionImportSTL, &QAction::triggered, [this]() {
	// 	FileDialogUtils::executeWithFileSelection([this](const QString fname) { _modelInterface->importSTEP(fname, progressBar);}, "Import", FileDialogUtils::FilterSTL);
	// });

	connect(&this->buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
		this, &MainWindow::handleSelectorButtonClicked);
		
	// connect(ui->actionGenerateMesh, &QAction::triggered, [this]() {
	// 	generateMesh();
	// });
	connect(ui->actionShowMesh, &QAction::toggled, [this](bool checked) {
		if (checked) {
			showMesh();
		} else {
			showGeometry();
		}
	});

	connect(this->ui->treeWidget, &QTreeWidget::itemSelectionChanged,
		this, &MainWindow::onItemSelectionChanged, Qt::DirectConnection);
}

//----------------------------------------------------------------------------
void MainWindow::connectModelToRenderWindow(ModelActionsHandler* aModelHandler, Rendering::RenderSignalHandler* aRenderHandler){
	//TODO: unify the handler so that both are set in the same way (now one is a field, the other via method)
	//TODO: Fun task - encapsulate the connections in a helper map/class/namespace 
	GeometryActionsHandler* geoActions = aModelHandler->_geometryHandler;
	Rendering::GeometryRenderHandler* geoRender = aRenderHandler->geometry();

    // QObject::connect(geoActions, &GeometryActionsHandler::addShape,
    //                  geoRender, &Rendering::GeometryRenderHandler::shapeAdded);

    // QObject::connect(geoActions, &GeometryActionsHandler::removeShape,
    //                  geoRender, &Rendering::GeometryRenderHandler::shapeRemoved);

    // QObject::connect(geoActions, &GeometryActionsHandler::modifyShape,
    //                  geoRender, &Rendering::GeometryRenderHandler::shapeModified);



    QObject::connect(geoActions, &GeometryActionsHandler::addShapes,
                     geoRender, &Rendering::GeometryRenderHandler::shapesAdded);

    // QObject::connect(geoActions, &GeometryActionsHandler::removeShapes,
    //                  geoRender, &Rendering::GeometryRenderHandler::shapesRemoved);

    // QObject::connect(geoActions, &GeometryActionsHandler::modifyShapes,
    //                  geoRender, &Rendering::GeometryRenderHandler::shapesModified);


	// MeshActionsHandler* meshHandler = aModelHandler->_meshHandler;
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

void MainWindow::initializeActions() {
	ui->actionImportSTEP->setEnabled(false);
	ui->actionImportSTL->setEnabled(false);
	ui->actionGenerateMesh->setEnabled(false);
}

void MainWindow::showMesh() {
	this->QVTKRender->clearRenderer();
	// this->QVTKRender->addActor(this->model->getMeshActor());
	this->QVTKRender->RenderScene();
}
void MainWindow::showGeometry(){
	this->QVTKRender->clearRenderer();
	this->QVTKRender->addPipelinesToRenderer();
	this->QVTKRender->RenderScene();
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
