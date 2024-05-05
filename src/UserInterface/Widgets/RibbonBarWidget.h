#ifndef RIBBONBARWIDGET_H
#define RIBBONBARWIDGET_H

#include "SARibbon.h"

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

public:
	QVBoxLayout* verticalLayout;

private:
	void buildRibbon(SARibbonBar* bar);
	QAction* createAction(const QString& text, const QString& iconurl);
	void setupUi();

private slots:
	void onRibbonThemeComboBoxCurrentIndexChanged(int index);

private:
	SARibbonBar* mRibbonBar { nullptr };
	QComboBox* mComboTheme { nullptr };
};

#endif