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

	// Getting acces to main window instance
	_mainWindow = qobject_cast<MainWindow*>(parent->window());

	QComboBox* comboBox = this->ui->appThemeComboBox;
	for (const auto& Theme : _advancedStyleSheet->themes()) {
		comboBox->addItem(Theme);
	}

	this->intitializeColorProperties();
	this->updateThemeColorButtons();
	this->setConnections();
}

//--------------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog() {
	delete ui;
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::setConnections() {
	connect(this->ui->appThemeComboBox, &QComboBox::currentTextChanged,
		this, &PreferencesDialog::updateStyleSheet);

	connect(this->ui->renBackgroundComboBox, &QComboBox::currentTextChanged,
		this, &PreferencesDialog::onRenBackgroundComboBoxChanged);

	connect(this->ui->applyButton, &QPushButton::clicked,
		this, &PreferencesDialog::onApplyButtonClicked);
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

//--------------------------------------------------------------------------------------
void PreferencesDialog::updateThemeColorButtons() {
	QGridLayout* appColorsLayout = qobject_cast<QGridLayout*>(ui->appColorsWidget->layout());
	if (!appColorsLayout) {
		qWarning() << "No QGridLayout found in QGroupBox!";
		return;
	}

	const auto& ThemeColors = _advancedStyleSheet->themeColorVariables();
	auto itc = ThemeColors.constBegin();
	for (int i = 0; i < appColorsLayout->count(); ++i, ++itc) {
		QWidget* widget = appColorsLayout->itemAt(i)->widget();
		ColorPickerWidget* colorPicker = qobject_cast<ColorPickerWidget*>(widget);

		if (colorPicker) {
			QColor color = itc.value();
			std::tuple<int, int, int> colorTuple(color.red(), color.green(), color.blue());
			colorPicker->setValue(colorTuple);
		}
	}
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::updateStyleSheet(QString theme) {
	_advancedStyleSheet->setCurrentTheme(theme);
	_advancedStyleSheet->updateStylesheet();
	qApp->setStyleSheet(_advancedStyleSheet->styleSheet());

	this->updateThemeColorButtons();
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::onApplyButtonClicked() {
	Rendering::QVTKRenderWindow* aRenWin = _mainWindow->getRenderWindow();
	int index = this->ui->renBackgroundComboBox->currentIndex();
	if (index == 0) {
		aRenWin->setBackground(ui->firstRenColor->getColorAsDoubleArray());

	} else {
		const double* col1 = ui->firstRenColor->getColorAsDoubleArray();
		const double* col2 = ui->secRenColor->getColorAsDoubleArray();
		vtkRenderer::GradientModes mode;

		switch (index) {
		case 1:
			mode = vtkRenderer::GradientModes::VTK_GRADIENT_VERTICAL;
			break;
		case 2:
			mode = vtkRenderer::GradientModes::VTK_GRADIENT_HORIZONTAL;
			break;
		case 3:
			mode = vtkRenderer::GradientModes::VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_SIDE;
			break;
		case 4:
			mode = vtkRenderer::GradientModes::VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_CORNER;
			break;
		default:
			qWarning() << "Wrong index!";
			return;
		}

		aRenWin->setBackground(mode, col1, col2);
	}
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::onRenBackgroundComboBoxChanged(QString text) {
	if (text == "Single Color") {
		this->ui->secRenColor->setEnabled(false);
		this->ui->secRenColorLabel->setEnabled(false);

	} else {
		this->ui->secRenColor->setEnabled(true);
		this->ui->secRenColorLabel->setEnabled(true);
	}
}