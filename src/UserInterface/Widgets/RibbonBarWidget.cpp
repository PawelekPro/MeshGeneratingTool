
#include "RibbonBarWidget.h"

RibbonBarWidget::RibbonBarWidget(QWidget* parent)
	: QWidget(parent) {

	this->setupUi();

	_ribbonBar = new SARibbonBar(parent);
	_ribbonBar->setTitleVisible(false);
	_ribbonBar->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
	_ribbonBar->setApplicationButton(nullptr);

#ifdef _WIN32
	sa_set_ribbon_theme(_ribbonBar, SARibbonTheme::RibbonThemeOffice2021Blue);
#endif

#ifdef linux
	sa_set_ribbon_theme(_ribbonBar, SARibbonTheme::RibbonThemeDark2);
#endif

	this->verticalLayout->setMenuBar(_ribbonBar);
	buildRibbon(_ribbonBar);
}

RibbonBarWidget::~RibbonBarWidget() {
	delete verticalLayout;
	delete _ribbonBar;
}

void RibbonBarWidget::buildRibbon(SARibbonBar* bar) {
	SARibbonCategory* page1 = new SARibbonCategory();
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
	pannel2->addLargeAction(createAction("setting", ":/icons/icons/customize0.svg"));
	pannel2->addLargeAction(createAction("windowsflag", ":/icons/icons/windowsflag-normal.svg"));
	bar->addCategoryPage(page1);

	SARibbonCategory* page2 = new SARibbonCategory();
	page2->setCategoryName("Mesh");
	bar->addCategoryPage(page2);

	SARibbonCategory* page3 = new SARibbonCategory();
	page3->setCategoryName("Display");
	bar->addCategoryPage(page3);

	/* ================================================================
	Selection page
	===================================================================*/
	SARibbonCategory* page4 = new SARibbonCategory();
	page4->setCategoryName("Selection");
	bar->addCategoryPage(page4);

	SARibbonPannel* selectionPanel = new SARibbonPannel("Selection type", page4);
	page4->addPannel(selectionPanel);

	QAction* vertexSelAct = createAction("Vertex", ":/icons/icons/Selection_vertex.svg");
	vertexSelAct->setIconText("Vertex");
	selectionPanel->addLargeAction(vertexSelAct);

	QAction* edgeSelAct = createAction("Edge", ":/icons/icons/Selection_edge.svg");
	edgeSelAct->setIconText("Edge");
	selectionPanel->addLargeAction(edgeSelAct);

	QAction* faceSelAct = createAction("Face", ":/icons/icons/Selection_face.svg");
	faceSelAct->setIconText("Face");
	selectionPanel->addLargeAction(faceSelAct);

	QAction* volSelAct = createAction("Volume", ":/icons/icons/Selection_volume.svg");
	volSelAct->setIconText("Volume");
	selectionPanel->addLargeAction(volSelAct);

	/* ================================================================
	Ribbon bar theme selection
	===================================================================*/
	mComboTheme = new QComboBox();
	mComboTheme->addItem("Theme Win7", static_cast<int>(SARibbonTheme::RibbonThemeWindows7));
	mComboTheme->addItem("Theme Office2013", static_cast<int>(SARibbonTheme::RibbonThemeOffice2013));
	mComboTheme->addItem("Theme Office2016 Blue", static_cast<int>(SARibbonTheme::RibbonThemeOffice2016Blue));
	mComboTheme->addItem("Theme Office2021 Blue", static_cast<int>(SARibbonTheme::RibbonThemeOffice2021Blue));
	mComboTheme->addItem("Theme Dark", static_cast<int>(SARibbonTheme::RibbonThemeDark));
	mComboTheme->addItem("Theme Dark2", static_cast<int>(SARibbonTheme::RibbonThemeDark2));
	mComboTheme->setCurrentIndex(mComboTheme->findData(static_cast<int>(SARibbonTheme::RibbonThemeOffice2013)));
	connect(mComboTheme,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&RibbonBarWidget::onRibbonThemeComboBoxCurrentIndexChanged);
	pannel2->addSeparator();
	pannel2->addSmallWidget(mComboTheme);

	SARibbonQuickAccessBar* qbar = _ribbonBar->quickAccessBar();
	// qbar->addAction(createAction("undo", ":/icons/icons/undo.svg"));
	// qbar->addAction(createAction("redo", ":/icons/icons/redo.svg"));
}

QAction* RibbonBarWidget::createAction(const QString& text, const QString& iconurl) {
	QAction* act = new QAction(this);
	act->setText(text);
	act->setIcon(QIcon(iconurl));
	act->setObjectName(text);
	return act;
}

void RibbonBarWidget::onRibbonThemeComboBoxCurrentIndexChanged(int index) {
	SARibbonTheme t = static_cast<SARibbonTheme>(mComboTheme->itemData(index).toInt());
	sa_set_ribbon_theme(_ribbonBar, t);
	_ribbonBar->updateRibbonGeometry();
}

void RibbonBarWidget::setupUi() {

	this->verticalLayout = new QVBoxLayout(this);
	this->verticalLayout->setSpacing(1);
	this->verticalLayout->setObjectName("verticalLayout");
	this->verticalLayout->setContentsMargins(0, 0, 0, 0);

	QMetaObject::connectSlotsByName(this);
}