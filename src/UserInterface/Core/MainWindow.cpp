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
<<<<<<< HEAD
	, _modelInterface(aModelInterface) {

=======
	, _modelInterface(aModelInterface)
{
	
>>>>>>> d50f60d ([WIP] compilable finalized ui structure - needs implementation)
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
	_modelHandler = new ModelActionsHandler(_modelInterface, _renderSignalSender, progressBar, this);
	this->connectModelToRenderWindow(_renderSignalSender, _renderSignalHandler);

 	this->ui->statusBar->addWidget(progressBar);

	this->ui->treeWidget->setModelHandler(_modelHandler);

	this->setConnections();
	this->initializeActions();

	AppTheme& appTheme = AppTheme::getInstance();
	appTheme.initializeAppStylesheet();
	Model::initializeGmsh();
	newModel();

	// JsonParser jsonParser;
	// const rapidjson::Document document = jsonParser.initJsonDocumnet(AppDefaults::getInstance().getDefaultPropertiesPath());
	// jsonParser.parseEntryProperties(document, DocumentHandler::entryTags.value(DocumentHandler::EntryTag::MeshSizing));
}
//----------------------------------------------------------------------------
MainWindow::~MainWindow() {
	QObject::disconnect(this->ui->treeWidget, &QTreeWidget::itemSelectionChanged,
		this, &MainWindow::onItemSelectionChanged);

	delete _modelHandler;
	delete documentHandler;
	delete QVTKRender;
	delete progressBar;
	delete ui;
}

//----------------------------------------------------------------------------
void MainWindow::setConnections() {
<<<<<<< HEAD
<<<<<<< HEAD
	connect(ui->actionImportSTEP, &QAction::triggered, [this]() {
		FileDialogUtils::executeWithFileSelection([this](const QString fname) { _modelInterface->importSTEP(fname, progressBar); }, "Import", FileDialogUtils::FilterSTEP);
	});

	connect(ui->actionImportSTL, &QAction::triggered, [this]() {
		FileDialogUtils::executeWithFileSelection([this](const QString fname) { _modelInterface->importSTEP(fname, progressBar); }, "Import", FileDialogUtils::FilterSTL);
	});

	connect(&this->buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
		this, &MainWindow::handleSelectorButtonClicked);

	connect(ui->actionGenerateMesh, &QAction::triggered, [this]() {
		generateMesh();
	});
=======
	// connect(ui->actionImportSTEP, &QAction::triggered, [this]() {
	// 	FileDialogUtils::executeWithFileSelection([this](const QString fname) { _modelInterface->importSTEP(fname, progressBar);}, "Import", FileDialogUtils::FilterSTEP);
	// });
=======
>>>>>>> 2314c08 ([WIP] working state of signal handler)

	connect(ui->actionImportSTEP, &QAction::triggered, _modelHandler->_geometryHandler, &GeometryActionsHandler::importSTEP);

	connect(ui->actionImportSTL, &QAction::triggered, _modelHandler->_geometryHandler, &GeometryActionsHandler::importSTL);

	connect(&this->buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
		this, &MainWindow::handleSelectorButtonClicked);
		
	// connect(ui->actionGenerateMesh, &QAction::triggered, [this]() {
	// 	generateMesh();
	// });
>>>>>>> 86ad1fe ([WIP] added modelActionHandlers and modelCommands. Adjusted architecture to use those managers)
	connect(ui->actionShowMesh, &QAction::toggled, [this](bool checked) {
		if (checked) {
			showMesh();
		} else {
			showGeometry();
		}
	});

	connect(this->ui->treeWidget, &QTreeWidget::itemSelectionChanged,
		this, &MainWindow::onItemSelectionChanged, Qt::DirectConnection);

	connect(this->ui->treeWidget->eventHandler, &TreeWidgetEventHandler::entitySelectionConfirmed,
		this, [this]() {
			std::vector<std::reference_wrapper<const TopoDS_Shape>> selectedShapes = this->QVTKRender->getInteractorStyle()->getSelectedShapes();
			const std::vector<std::string> names = this->model->geometry.getShapesNames(selectedShapes);
			std::vector<int> selectedTags;
			for (auto shape : selectedShapes) {
				std::vector<int> newTags = this->model->geometry.getShapeVerticesTags(shape);
				selectedTags.insert(selectedTags.end(), newTags.begin(), newTags.end());
			}
			std::sort(selectedTags.begin(), selectedTags.end());
			auto last = std::unique(selectedTags.begin(), selectedTags.end());
			selectedTags.erase(last, selectedTags.end());
			emit this->ui->treeWidget->eventHandler->selectedEntitiesNamesFetched(names, selectedTags);
		});
}

//----------------------------------------------------------------------------
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
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
		for (auto shape : shapesMap) {
			this->QVTKRender->addShapeToRenderer(shape.second);

			mgtm = vtkSmartPointer<MGTMesh>::New();
			npm = std::make_unique<NetgenPlugin_Mesher>(mgtm.GetPointer(), shape.second, true);
			npm->ComputeMesh();

			auto boundaryMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			boundaryMapper->SetInputData(mgtm->GetBoundaryMesh());
			auto boundaryActor = vtkSmartPointer<vtkActor>::New();
			boundaryActor->SetMapper(boundaryMapper);
			boundaryActor->GetProperty()->SetEdgeVisibility(true);
			boundaryActor->GetProperty()->SetEdgeColor(1.0, 1.0, 1.0);
			boundaryActor->GetProperty()->SetColor(0.4, 0.6, 0.8);
			QVTKRender->addActor(boundaryActor);
			QVTKRender->fitView();
		}
	} catch (std::filesystem::filesystem_error) {
		this->progressBar->setTerminateIndicator(false);
		std::cout << "Display some message or dialog box..." << std::endl;
		return;
	}

	QFileInfo fileInfo(fileName);
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
	PropertiesList propList = this->ui->treeWidget->getRootProperties(TreeStructure::TreeRoot::Mesh);
	double minElementSize = 1;
	double maxElementSize = 5;
	for (auto& propMap : propList) {
		if (propMap.value("name") == "minElementSize") {
			minElementSize = propMap.value("value").toDouble();
		}
		if (propMap.value("name") == "maxElementSize") {
			maxElementSize = propMap.value("value").toDouble();
		}
	}
	QMap<QString, PropertiesList> sizingMap = this->ui->treeWidget->getItemsProperties(TreeStructure::TreeRoot::Mesh,
		TreeStructure::XMLTag::MeshSizing);

	for (auto& propList : sizingMap) {
		std::vector<int> verticesTags;
		double size;
		for (auto& propMap : propList) {
			if (propMap.value("name") == "selectedTags") {
				QString tagString = propMap.value("value");
				QStringList tagList = tagString.split(',');
				for (QString& s : tagList) {
					if (s == ',') {
						continue;
					} else {
						verticesTags.push_back(s.toInt());
					}
				}
			}
			if (propMap.value("name") == "elementSize") {
				size = propMap.value("value").toFloat();
			}
		}
		this->model->addSizing(verticesTags, size);
	}

	// this->model->fetchMeshProperties(minElementSize, maxElementSize);
	// this->model->meshSurface();
}
//----------------------------------------------------------------------------
=======
>>>>>>> 86ad1fe ([WIP] added modelActionHandlers and modelCommands. Adjusted architecture to use those managers)
=======
void MainWindow::connectModelToRenderWindow(ModelActionsHandler* aModelHandler, Rendering::RenderSignalHandler* aRenderHandler){
=======
void MainWindow::connectModelToRenderWindow(RenderSignalSender* aSignalSender, Rendering::RenderSignalHandler* aSignalHandler){
>>>>>>> 2314c08 ([WIP] working state of signal handler)
	//TODO: unify the handler so that both are set in the same way (now one is a field, the other via method)
	//TODO: Fun task - encapsulate the connections in a helper map/class/namespace 
	GeometrySignalSender* geoSender = aSignalSender->geometrySignals;
	Rendering::GeometryRenderHandler* geoRender = aSignalHandler->geometry();

    QObject::connect(geoSender, &GeometrySignalSender::addShapes,
                     geoRender, &Rendering::GeometryRenderHandler::shapesAdded);
}

//----------------------------------------------------------------------------
>>>>>>> 4e969e1 ([WIP] added working render signal receiver)
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
	// ui->actionImportSTEP->setEnabled(false);
	ui->actionImportSTL->setEnabled(false);
	ui->actionGenerateMesh->setEnabled(false);
}

void MainWindow::showMesh() {
	this->QVTKRender->clearRenderer();
	// this->QVTKRender->addActor(this->model->getMeshActor());
	this->QVTKRender->RenderScene();
}
void MainWindow::showGeometry() {
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
