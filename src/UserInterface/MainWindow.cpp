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

	this->progressBar = new ProgressBar(this);
	this->ui->statusBar->addWidget(progressBar);

	this->buttonGroup.addButton(this->ui->facesSelectorButton, 0);
	this->buttonGroup.addButton(this->ui->edgesSelectorButton, 1);
	
	this->setConnections();
	this->initializeActions();
	}
//----------------------------------------------------------------------------
MainWindow::~MainWindow() {
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
	connect(ui->actionShowMesh, &QAction::triggered, [this]() {
		showMesh();
	});
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
	ProgressBar* progressBar = this->getProgressBar();
	STEPGeometryPlugin stepReader {};

	const std::string& filePath = fileName.toStdString();
	try {
		stepReader.load(filePath, this->progressBar);
	} catch (std::filesystem::filesystem_error) {
		this->progressBar->setTerminateIndicator(false);
		std::cout << "Display some message or dialog box..." << std::endl;
		return;
	}
	this->model->addParts(stepReader.stepOperations.getPartsMap());
	Geometry::ActorsMap actorsMap = stepReader.getVTKActorsMap();
	QVTKRender->addActors(actorsMap, true);
	QVTKRender->fitView();
	// Geometry::ActorsMap edgesMap = stepReader.getVTKEdgesMap();
	// QVTKRender->addEdgesActors(edgesMap);
	// QVTKRender->setActiveLayerRenderer(0);
}

//----------------------------------------------------------------------------
void MainWindow::importSTL(QString fileName) {
	ProgressBar* progressBar = this->getProgressBar();
	STLPlugin::STLFileReader stlReader {};

	const std::string& filePath = fileName.toStdString();
	stlReader.load(filePath, progressBar);
	this->model->addParts(stlReader.getPartsMap());

	Geometry::ActorsMap actorsMap = stlReader.getVTKActorsMap();
	QVTKRender->addActors(actorsMap);
	QVTKRender->fitView();
}
//----------------------------------------------------------------------------
void MainWindow::newModel() {
	std::string  modelName = "Model_1";
	this->model = std::make_unique<Model>(modelName);

	// enable imports
	ui->actionImportSTEP->setEnabled(true);
	ui->actionImportSTL->setEnabled(true);
	ui->actionGenerateMesh->setEnabled(true);

}
void MainWindow::generateMesh() {
	this->model->meshParts();
}
//----------------------------------------------------------------------------
void MainWindow::handleSelectorButtonClicked(QAbstractButton* button) {
	for (QAbstractButton* btn : this->buttonGroup.buttons()) {
		if (btn != button)
			btn->setChecked(false);
	}

	// Set active layer according to selected button
	// qDebug() << "Button" << buttonGroup.id(button) << "clicked";
	this->QVTKRender->setActiveLayerRenderer(buttonGroup.id(button));
}

void MainWindow::initializeActions(){
	ui->actionImportSTEP->setEnabled(false);
	ui->actionImportSTL->setEnabled(false);
	ui->actionGenerateMesh->setEnabled(false);
}

void MainWindow::showMesh(){

	// this->QVTKRender->getRenderer()


	this->model->updateMeshActor();
	this->QVTKRender->addActor(this->model->meshActor);
	this->QVTKRender->getRenderer()->Render();
}