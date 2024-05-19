#ifndef RIBBONBARWIDGET_H
#define RIBBONBARWIDGET_H

#include "MainWindow.h"
#include "SARibbon.h"

#include <QActionGroup>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class SARibbonBar;
class RibbonBarWidget : public QWidget {
	Q_OBJECT

public:
	RibbonBarWidget(QWidget* parent = nullptr);
	~RibbonBarWidget();
	void initialize() {
		_QVTKRender = _mainWindow->getRenderWindow();
	};

public:
	QVBoxLayout* verticalLayout;

private:
	void buildRibbon(SARibbonBar* bar);
	void setupUi();

	QAction* createAction(const QString& text, const QString& iconurl);

private slots:
	void onRibbonThemeComboBoxCurrentIndexChanged(int index);
	void onEntitySelectionChanged(QAction*);

private:
	MainWindow* _mainWindow;
	Rendering::QVTKRenderWindow* _QVTKRender;
	SARibbonBar* _ribbonBar { nullptr };
	QComboBox* mComboTheme { nullptr };
};

#endif