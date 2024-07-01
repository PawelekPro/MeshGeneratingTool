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
PreferencesDialog::PreferencesDialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::PreferencesDialog) {
	ui->setupUi(this);

	// Getting acces to main window instance
	_mainWindow = qobject_cast<MainWindow*>(parent->window());

	QComboBox* comboBox = this->ui->appThemeComboBox;
	for (const auto& Theme : AppTheme::getInstance().themes()) {
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
	AppDefaults& appDefaults = AppDefaults::getInstance();
	const AppDefaultColors::GeomColorsArray colorsArray = appDefaults.getGeometryEntitiesColorArray();

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

	const AppDefaultColors::RendererColorsArray renColorsArr = appDefaults.getRendererColorsArray();
	QColor color1 = renColorsArr.at(0);
	QColor color2 = renColorsArr.at(1);

	std::tuple<int, int, int> colorTuple1(color1.red(), color1.green(), color1.blue());
	std::tuple<int, int, int> colorTuple2(color2.red(), color2.green(), color2.blue());

	ui->firstRenColor->setValue(colorTuple1);
	ui->secRenColor->setValue(colorTuple2);

	const bool isGradModeEnabled = appDefaults.isGradientBackgroundEnabled();
	int index = 0;
	if (isGradModeEnabled) {
		vtkRenderer::GradientModes mode = appDefaults.getRendererGradientMode();
		index = static_cast<int>(mode) + 1;
		ui->secRenColor->setEnabled(true);
		ui->secRenColorLabel->setEnabled(true);
	}
	ui->renBackgroundComboBox->setCurrentIndex(index);
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::updateThemeColorButtons() {
	QGridLayout* appColorsLayout = qobject_cast<QGridLayout*>(ui->appColorsWidget->layout());
	if (!appColorsLayout) {
		qWarning() << "No QGridLayout found in QGroupBox!";
		return;
	}

	const auto& ThemeColors = AppTheme::getInstance().themeColorVariables();
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
	AppTheme& appTheme = AppTheme::getInstance();
	appTheme.setCurrentTheme(theme);
	appTheme.updateStylesheet();
	appTheme.updateAppStylesheet();

	this->updateThemeColorButtons();
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::onApplyButtonClicked() {
	this->processRendererSettings();
	this->close();
}

//--------------------------------------------------------------------------------------
void PreferencesDialog::processRendererSettings() {
	Rendering::QVTKRenderWindow* aRenWin = _mainWindow->getRenderWindow();
	AppDefaults& appDefaults = AppDefaults::getInstance();

	int index = this->ui->renBackgroundComboBox->currentIndex();
	const double* col1 = ui->firstRenColor->getColorAsDoubleArray();
	const double* col2 = ui->secRenColor->getColorAsDoubleArray();

	if (index == 0) {
		aRenWin->setBackground(ui->firstRenColor->getColorAsDoubleArray());

		appDefaults.setGradientBackgroundEnabled(false);
		appDefaults.setRendererColorsArray(
			AppDefaultColors::doubleColorsToColorsArray(col1, col2));

	} else {
		vtkRenderer::GradientModes mode;

		switch (index) {
		case 1:
			mode = vtkRenderer::GradientModes::
				VTK_GRADIENT_VERTICAL;
			break;
		case 2:
			mode = vtkRenderer::GradientModes::
				VTK_GRADIENT_HORIZONTAL;
			break;
		case 3:
			mode = vtkRenderer::GradientModes::
				VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_SIDE;
			break;
		case 4:
			mode = vtkRenderer::GradientModes::
				VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_CORNER;
			break;
		default:
			qWarning() << "Wrong index!";
			return;
		}

		aRenWin->setBackground(mode, col1, col2);

		appDefaults.setRendererGradientMode(mode);
		appDefaults.setGradientBackgroundEnabled(true);
		appDefaults.setRendererColorsArray(
			AppDefaultColors::doubleColorsToColorsArray(col1, col2));
	}

	appDefaults.updateRendererSettings();
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