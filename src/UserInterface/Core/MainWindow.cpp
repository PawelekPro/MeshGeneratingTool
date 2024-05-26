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

#include "MainWindow.h"
#include "./ui_MainWindow.h"

//----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	ui->setupUi(this);

	// Set initial sizes of the splitter sections
	QList<int> sizes;
	sizes << 100 << 400;
	ui->mainSplitter->setSizes(sizes);

	this->QVTKRender = new Rendering::QVTKRenderWindow(ui->modelView);
	this->QVTKRender->enableCameraOrientationWidget();
	this->QVTKRender->enableWaterMark();
	this->ui->ribbonBar->initialize();

	this->progressBar = new ProgressBar(this);
	this->ui->statusBar->addWidget(progressBar);

	// this->buttonGroup.addButton(this->ui->volumeSelectorButton,
	// 	static_cast<int>(Rendering::Renderers::Main));

	// this->buttonGroup.addButton(this->ui->facesSelectorButton,
	// 	static_cast<int>(Rendering::Renderers::Faces));

	// this->buttonGroup.addButton(this->ui->edgesSelectorButton,
	// 	static_cast<int>(Rendering::Renderers::Edges));

	this->setConnections();
	this->initializeActions();
}
//----------------------------------------------------------------------------
MainWindow::~MainWindow() {
	QObject::disconnect(this->ui->treeWidget, &QTreeWidget::itemSelectionChanged,
		this, &MainWindow::onItemSelectionChanged);

	delete QVTKRender;
	delete progressBar;
	delete ui;
}

//----------------------------------------------------------------------------
void MainWindow::setConnections() {
	connect(ui->actionImportSTEP, &QAction::triggered, [this]() {
		openFileDialog([this](QString fname) { importSTEP(fname); }, "Import", filters::StepFilter);
	});

	connect(ui->actionImportSTL, &QAction::triggered, [this]() {
		openFileDialog([this](QString fname) { importSTL(fname); }, "Import", filters::StlFilter);
	});

	connect(&this->buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
		this, &MainWindow::handleSelectorButtonClicked);

	connect(ui->actionNewModel, &QAction::triggered, [this]() {
		newModel();
	});
	connect(ui->actionGenerateMesh, &QAction::triggered, [this]() {
		generateMesh();
	});
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
int MainWindow::openFileDialog(Callable action, QString actionName, QString filter) {
	QFileDialog dlg(this);
	dlg.setWindowTitle("Select file to " + actionName);
	dlg.setNameFilter(filter);

	QString fname = dlg.getOpenFileName(this, actionName, "", filter);

	if (!fname.isEmpty()) {
		action(fname);
		return QMessageBox::Accepted;
	}
	return QMessageBox::Rejected;
}

//----------------------------------------------------------------------------
void MainWindow::importSTEP(QString fileName) {
	QPointer<ProgressBar> progressBar = this->getProgressBar();
	const std::string& filePath = fileName.toStdString();
	try {
		this->model->importSTEP(filePath, this->progressBar);
		const GeometryCore::PartsMap& shapesMap = model->geometry.getShapesMap();
		for(auto shape : shapesMap){
			this->QVTKRender->addShapeToRenderer(shape.second);
		}
	} catch (std::filesystem::filesystem_error) {
		this->progressBar->setTerminateIndicator(false);
		std::cout << "Display some message or dialog box..." << std::endl;
		return;
	}

	QFileInfo fileInfo(fileName);
	this->ui->treeWidget->loadGeometryFile(fileInfo.baseName());
	QVTKRender->fitView();
}

//----------------------------------------------------------------------------
void MainWindow::importSTL(QString fileName) {
	ProgressBar* progressBar = this->getProgressBar();

	const std::string& filePath = fileName.toStdString();
	this->model->geometry.importSTL(filePath, this->progressBar);
	// this->QVTKRender->updateGeometryActors(this->model->geometry);

	QVTKRender->fitView();
}
//----------------------------------------------------------------------------
void MainWindow::newModel() {
	std::string modelName = "Model_1";
	this->model = std::make_shared<Model>(modelName);
	this->QVTKRender->model = this->model;
	// enable imports
	ui->actionImportSTEP->setEnabled(true);
	ui->actionImportSTL->setEnabled(true);
	ui->actionGenerateMesh->setEnabled(true);
}
void MainWindow::generateMesh() {
	this->model->meshSurface();
	this->ui->treeWidget->addMeshSizing();
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
	this->QVTKRender->addActor(this->model->getMeshActor());
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

	QTreeWidgetItem* item;
	if (!itemsList.isEmpty()) {
		item = itemsList.takeFirst();
	}

	QVariant modelVariant = item->data(
		0, TreeStructure::Role.value("Properties"));
	QSharedPointer<PropertiesModel> sharedModel
		= modelVariant.value<QSharedPointer<PropertiesModel>>();

	if (!sharedModel.isNull()) {
		PropertiesModel* model = sharedModel.data();
		// ToDo: Detect data changed event and make project unsaved

		this->ui->propertiesTable->setModel(model);
		QHeaderView* header = this->ui->propertiesTable->horizontalHeader();
		header->setSectionResizeMode(QHeaderView::ResizeToContents);
		header->setStretchLastSection(true);
		header->setSectionResizeMode(QHeaderView::Interactive);
	}
}
