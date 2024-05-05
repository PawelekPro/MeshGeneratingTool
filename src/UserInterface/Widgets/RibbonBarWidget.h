#ifndef RIBBONBARWIDGET_H
#define RIBBONBARWIDGET_H

#include <QComboBox>
#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class RibbonBarWidget;
}
QT_END_NAMESPACE

class SARibbonBar;
class RibbonBarWidget : public QWidget {
	Q_OBJECT

public:
	RibbonBarWidget(QWidget* parent = nullptr);
	~RibbonBarWidget();

private:
	void buildRibbon(SARibbonBar* bar);
	QAction* createAction(const QString& text, const QString& iconurl);
private slots:
	void onRibbonThemeComboBoxCurrentIndexChanged(int index);

private:
	Ui::RibbonBarWidget* ui;
	SARibbonBar* mRibbonBar { nullptr };
	QComboBox* mComboTheme { nullptr };
};

#endif