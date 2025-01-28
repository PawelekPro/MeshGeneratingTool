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

#include "RibbonBarWidget.hpp"
#include "MeshActionsHandler.hpp"
#include "ModelActionsHandler.hpp"

//----------------------------------------------------------------------------
RibbonBarWidget::RibbonBarWidget(QWidget* parent)
	: QWidget(parent)
	, _modelHandler(nullptr) {

	this->setupUi();

	// Getting acces to main window instance
	_mainWindow = qobject_cast<MainWindow*>(parent->window());

	_ribbonBar = new SARibbonBar(parent);
	_ribbonBar->setTitleVisible(false);
	_ribbonBar->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
	_ribbonBar->setApplicationButton(nullptr);

	sa_set_ribbon_theme(_ribbonBar, SARibbonTheme::Default);
	this->verticalLayout->setMenuBar(_ribbonBar);
}

//----------------------------------------------------------------------------
void RibbonBarWidget::initialize() {
	_QVTKRender = _mainWindow->getRenderWindow();
	this->buildRibbon(_ribbonBar);
}

//----------------------------------------------------------------------------
RibbonBarWidget::~RibbonBarWidget() {
	delete verticalLayout;
	delete _ribbonBar;
}

//----------------------------------------------------------------------------
void RibbonBarWidget::buildRibbon(SARibbonBar* bar) {
	SARibbonCategory* page1 = new SARibbonCategory(this);
	page1->setCategoryName("Home");
	SARibbonPannel* pannel1 = new SARibbonPannel("pannel1", page1);
	page1->addPannel(pannel1);
	QAction* act = createAction("  save  ", ":/icons/icons/save.svg");
	act->setIconText("  save  ");
	pannel1->addLargeAction(act);

	pannel1->addLargeAction(createAction("open", ":/icons/icons/folder-star.svg"));
	pannel1->addSmallAction(createAction("action1", ":/icons/icons/action.svg"));
	pannel1->addSmallAction(createAction("action2", ":/icons/icons/action2.svg"));
	SARibbonPannel* pannel2 = new SARibbonPannel("pannel2", page1);
	page1->addPannel(pannel2);

	QAction* settingsAct = createAction("setting", ":/icons/icons/customize0.svg");
	connect(settingsAct, &QAction::triggered, _mainWindow, &MainWindow::onShowDialogButtonClicked);

	pannel2->addLargeAction(settingsAct);

	pannel2->addLargeAction(createAction("windowsflag", ":/icons/icons/windowsflag-normal.svg"));
	bar->addCategoryPage(page1);

	/* ================================================================
	  Mesh page
	  ===================================================================*/
	SARibbonCategory* page2 = new SARibbonCategory(this);
	page2->setCategoryName("Mesh");
	bar->addCategoryPage(page2);

	SARibbonPannel* meshGenerationPanel = new SARibbonPannel("Generate mesh", page2);
	page2->addPannel(meshGenerationPanel);
	QAction* generate3DMeshAction = createAction("generate3DMesh", ":/icons/icons/Volume_mesh.svg");
	generate3DMeshAction->setIconText("Volume mesh");
	meshGenerationPanel->addAction(generate3DMeshAction);
	connect(generate3DMeshAction, &QAction::triggered, _modelHandler->_meshHandler,
		&MeshActionsHandler::generate3DMesh);

	QAction* generate2DMeshAction
		= createAction("generate2DMesh", ":/icons/icons/Surface_mesh.svg");
	generate2DMeshAction->setIconText("Surface mesh");
	meshGenerationPanel->addAction(generate2DMeshAction);
	connect(generate2DMeshAction, &QAction::triggered, _modelHandler->_meshHandler,
		&MeshActionsHandler::generate2DMesh);

	/* ================================================================
	  Display page
	  ===================================================================*/
	SARibbonCategory* page3 = new SARibbonCategory(this);
	page3->setCategoryName("Display");
	bar->addCategoryPage(page3);

	SARibbonPannel* viewRepPanel = new SARibbonPannel("View representation", page3);
	page3->addPannel(viewRepPanel);

	QActionGroup* viewRepGroup = new QActionGroup(this);
	viewRepGroup->setExclusive(true);

	QAction* shadedRepAction = createAction("Shaded", ":/icons/icons/Shaded_representation.svg");
	shadedRepAction->setIconText("Shaded");
	shadedRepAction->setCheckable(true);
	viewRepGroup->addAction(shadedRepAction);
	viewRepPanel->addLargeAction(shadedRepAction);

	QAction* surfaceWithEdgesRepAction
		= createAction("Surface with edges", ":/icons/icons/Surface_with_edges_representation.svg");
	surfaceWithEdgesRepAction->setIconText("Surface with edges");
	surfaceWithEdgesRepAction->setCheckable(true);
	viewRepGroup->addAction(surfaceWithEdgesRepAction);
	viewRepPanel->addLargeAction(surfaceWithEdgesRepAction);

	QAction* wireframeRepAction
		= createAction("Wireframe", ":/icons/icons/Wireframe_representation.svg");
	wireframeRepAction->setIconText("Wireframe");
	wireframeRepAction->setCheckable(true);
	viewRepGroup->addAction(wireframeRepAction);
	viewRepPanel->addLargeAction(wireframeRepAction);

	QAction* pointsRepAction = createAction("Points", ":/icons/icons/Points_representation.svg");
	pointsRepAction->setIconText("Points");
	pointsRepAction->setCheckable(true);
	viewRepGroup->addAction(pointsRepAction);
	viewRepPanel->addLargeAction(pointsRepAction);

	connect(viewRepGroup, &QActionGroup::triggered, this,
		&RibbonBarWidget::onViewRepresentationChanged);

	/* ================================================================
	Selection page
	===================================================================*/
	SARibbonCategory* page4 = new SARibbonCategory(this);
	page4->setCategoryName("Selection");
	bar->addCategoryPage(page4);

	SARibbonPannel* selectionPanel = new SARibbonPannel("Selection type", page4);
	page4->addPannel(selectionPanel);

	QActionGroup* selectionGroup = new QActionGroup(this);
	selectionGroup->setExclusive(true);

	QAction* vertexSelAct = createAction("Vertex", ":/icons/icons/Selection_vertex.svg");
	vertexSelAct->setIconText("Vertex");
	vertexSelAct->setCheckable(true);
	selectionGroup->addAction(vertexSelAct);
	selectionPanel->addLargeAction(vertexSelAct);

	QAction* edgeSelAct = createAction("Edge", ":/icons/icons/Selection_edge.svg");
	edgeSelAct->setIconText("Edge");
	edgeSelAct->setCheckable(true);
	selectionGroup->addAction(edgeSelAct);
	selectionPanel->addLargeAction(edgeSelAct);

	QAction* faceSelAct = createAction("Face", ":/icons/icons/Selection_face.svg");
	faceSelAct->setIconText("Face");
	faceSelAct->setCheckable(true);
	selectionGroup->addAction(faceSelAct);
	selectionPanel->addLargeAction(faceSelAct);

	QAction* volSelAct = createAction("Volume", ":/icons/icons/Selection_volume.svg");
	volSelAct->setIconText("Volume");
	volSelAct->setCheckable(true);
	selectionGroup->addAction(volSelAct);
	selectionPanel->addLargeAction(volSelAct);

	connect(
		selectionGroup, &QActionGroup::triggered, this, &RibbonBarWidget::onEntitySelectionChanged);

	SARibbonQuickAccessBar* qbar = _ribbonBar->quickAccessBar();
	// qbar->addAction(createAction("undo", ":/icons/icons/undo.svg"));
	// qbar->addAction(createAction("redo", ":/icons/icons/redo.svg"));
}

//----------------------------------------------------------------------------
QAction* RibbonBarWidget::createAction(const QString& text, const QString& iconurl) {
	QAction* act = new QAction(this);
	act->setText(text);
	act->setIcon(QIcon(iconurl));
	act->setObjectName(text);
	return act;
}

//----------------------------------------------------------------------------
void RibbonBarWidget::setupUi() {

	this->verticalLayout = new QVBoxLayout(this);
	this->verticalLayout->setSpacing(1);
	this->verticalLayout->setObjectName("verticalLayout");
	this->verticalLayout->setContentsMargins(0, 0, 0, 0);

	QMetaObject::connectSlotsByName(this);
}

//----------------------------------------------------------------------------
void RibbonBarWidget::onEntitySelectionChanged(QAction* action) {
	const QString& actionText = action->text();

	if (actionText == "Vertex") {
		_QVTKRender->setSelectionMode(IVtk_SelectionMode::SM_Vertex);
	} else if (actionText == "Edge") {
		_QVTKRender->setSelectionMode(IVtk_SelectionMode::SM_Edge);
	} else if (actionText == "Face") {
		_QVTKRender->setSelectionMode(IVtk_SelectionMode::SM_Face);
	} else if (actionText == "Volume") {
		_QVTKRender->setSelectionMode(IVtk_SelectionMode::SM_Shape);
	}
}

//----------------------------------------------------------------------------
void RibbonBarWidget::onViewRepresentationChanged(QAction* action) {
	const QString& actionText = action->text();
	QIVtkViewRepresentation* viewRep = _QVTKRender->getViewRepresentation();

	if (actionText == "Shaded") {
		viewRep->setRepresentationToShaded();
	} else if (actionText == "Surface with edges") {
		viewRep->setRepresentationToSurfaceWithEdges();
	} else if (actionText == "Wireframe") {
		viewRep->setRepresentationToWireframe();
	} else if (actionText == "Points") {
		viewRep->setRepresentationToPoints();
	}

	_QVTKRender->RenderScene();
}

//----------------------------------------------------------------------------
void RibbonBarWidget::setModelHandler(ModelActionsHandler* aModelHandler) {
	if (aModelHandler) {
		_modelHandler = aModelHandler;
	}
}