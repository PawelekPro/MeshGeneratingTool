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

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	ui->setupUi(this);

	// Set initial sizes of the splitter sections
	QList<int> sizes;
	sizes << 100 << 400;
	ui->mainSplitter->setSizes(sizes);

	QVTKRender = new Rendering::QVTKRenderWindow(ui->modelView);
	QVTKRender->enableCameraOrientationWidget();
	// QVTKRender->enableButton();

	this->progressBar = new ProgressBar(this);
	this->ui->statusBar->addWidget(progressBar);

	this->setConnections();
}

MainWindow::~MainWindow() {
	delete QVTKRender;
	delete progressBar;
	delete ui;
}

void MainWindow::setConnections() {
	connect(ui->actionImportSTEP, &QAction::triggered, [this]() {
		openFileDialog([this](QString fname) { importSTEP(fname); }, "Import", filters::StepFilter);
	});

	connect(ui->actionImportSTL, &QAction::triggered, [this]() {
		openFileDialog([this](QString fname) { importSTL(fname); }, "Import", filters::StlFilter);
	});
}

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

void MainWindow::importSTEP(QString fileName) {
	ProgressBar* progressBar = this->getProgressBar();
	STEPPlugin::STEPPluginImport stepReader {};

	const std::string& filePath = fileName.toStdString();
	try {
		stepReader.load(filePath, this->progressBar);
	} catch (std::filesystem::filesystem_error) {
		this->progressBar->setTerminateIndicator(false);
		std::cout << "Display some message or dialog box..." << std::endl;
		return;
	}

	Geometry::ActorsMap actorsMap = stepReader.getVTKActorsMap();
	QVTKRender->addActors(actorsMap);

	Geometry::ActorsMap edgesMap = stepReader.getVTKEdgesMap();
	QVTKRender->addEdgesActors(edgesMap);
	QVTKRender->setActiveLayerRenderer(1);
}

void MainWindow::importSTL(QString fileName) {
	ProgressBar* progressBar = this->getProgressBar();
	STLPlugin::STLFileReader stlReader {};

	const std::string& filePath = fileName.toStdString();
	stlReader.load(filePath, progressBar);

	Geometry::ActorsMap actorsMap = stlReader.getVTKActorsMap();
	QVTKRender->addActors(actorsMap);
}