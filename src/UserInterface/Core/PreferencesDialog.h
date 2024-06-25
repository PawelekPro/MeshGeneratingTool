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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

class MainWindow;
#include "AppDefaults.h"
#include "ColorPickerWidget.h"
#include "MainWindow.h"

#include <QtAdvancedStylesheet.h>

#include <QColor>
#include <QDialog>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class PreferencesDialog;
}
QT_END_NAMESPACE

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	explicit PreferencesDialog(QWidget* parent = nullptr, acss::QtAdvancedStylesheet* styleSheet = nullptr);
	~PreferencesDialog();

private:
	void intitializeColorProperties();
	void updateThemeColorButtons();
	void updateStyleSheet(QString theme);
	void processRendererSettings();
	void setConnections();

private:
	Ui::PreferencesDialog* ui;
	acss::QtAdvancedStylesheet* _advancedStyleSheet;
	MainWindow* _mainWindow;

private slots:
	void onApplyButtonClicked();
	void onRenBackgroundComboBoxChanged(QString text);
};

#endif