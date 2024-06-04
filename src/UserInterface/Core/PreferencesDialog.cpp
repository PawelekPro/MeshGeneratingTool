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

#include "PreferencesDialog.h"
#include "./ui_PreferencesDialog.h"

//--------------------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog(QWidget* parent, acss::QtAdvancedStylesheet* styleSheet)
	: QDialog(parent)
	, ui(new Ui::PreferencesDialog)
	, _advancedStyleSheet(styleSheet) {
	ui->setupUi(this);

	QComboBox* comboBox = this->ui->appThemeComboBox;
	for (const auto& Theme : _advancedStyleSheet->themes()) {
		comboBox->addItem(Theme);
	}

	this->intitializeColorProperties();
}

//--------------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog() {
	delete ui;
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::intitializeColorProperties() {
	const AppDefaultColors::GeomColorsArray colorsArray
		= AppDefaults::getInstance().getGeometryEntitiesColorArray();

	QGridLayout* layout = qobject_cast<QGridLayout*>(ui->entityColorsWidget->layout());
	if (!layout) {
		qWarning() << "No QGridLayout found in QGroupBox!";
	} else {
		for (int i = 0; i < layout->count(); ++i) {
			QWidget* widget = layout->itemAt(i)->widget();
			ColorPickerWidget* colorPicker = qobject_cast<ColorPickerWidget*>(widget);

			if (colorPicker) {
				QColor color = colorsArray.at(i);
				std::tuple<int, int, int> colorTuple(color.red(), color.green(), color.blue());
				colorPicker->setValue(colorTuple);
			}
		}
	}
}