/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
 *
 * This file is part of the Mesh Generating Tool.
 * (https://github.com/PawelekPro/MeshGeneratingTool)
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

#include "AppSettings.h"

//--------------------------------------------------------------------------------------
AppSettings::AppSettings()
	: QSettings(QSettings::IniFormat, QSettings::UserScope, AppInfo::appName, AppInfo::appName) {

	QString settingsPath = QDir::toNativeSeparators(
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + ".ini");

	// If settings file does not exist then create file
	if (!QFile::exists(settingsPath)) {
		qDebug() << "Creating default settings.";
		this->createDefaultSettings();
	}

	this->loadDefaultSettings();
}

//--------------------------------------------------------------------------------------
void AppSettings::createDefaultSettings() {
	/* ================================================================
	Defaults
	===================================================================*/
	this->beginGroup(SettingsRoots::defaults);
	this->setValue("Name", AppInfo::appName);
	this->setValue("Version", AppInfo::appVersion);
	this->setValue("ProjFileVersion", AppInfo::appProjFileVersion);
	this->endGroup();

	/* ================================================================
	Theme
	===================================================================*/
	this->beginGroup(SettingsRoots::theme);
	this->endGroup();

	/* ================================================================
	Rendering
	===================================================================*/
	this->beginGroup(SettingsRoots::rendering);

	AppDefaultColors::RendererColorsArray renColorsArr
		= this->getRendererColorsArray(true);
	int index = 0;
	for (const QColor& color : renColorsArr) {
		QString colorVar = QString::fromStdString("color" + std::to_string(index++));
		this->setValue(colorVar, color);
	}

	this->setValue(
		"GradientBackgroundEnabled", this->isGradientBackgroundEnabled(true));
	this->setValue(
		"GradientBackgroundMode", static_cast<int>(this->getRendererGradientMode(true)));

	this->endGroup();

	/* ================================================================
	Graphics
	===================================================================*/
	this->beginGroup(SettingsRoots::graphics);

	AppDefaultColors::GeomColorsArray colorsArr
		= this->getGeometryEntitiesColorArray(true);
	index = 0;
	for (const QColor& color : colorsArr) {
		QString colorVar = QString::fromStdString("color" + std::to_string(index++));
		this->setValue(colorVar, color);
	}

	this->endGroup();
}

//--------------------------------------------------------------------------------------
void AppSettings::loadDefaultSettings() {
	/* ================================================================
	Graphics
	===================================================================*/
	this->beginGroup(SettingsRoots::graphics);

	// Reading geometry colors stored in settings file
	AppDefaultColors::GeomColorsArray colorsArr;
	for (size_t index = 0; index < colorsArr.size(); ++index) {
		QString colorVar = QString::fromStdString("color" + std::to_string(index));
		QColor color = this->value(colorVar).value<QColor>();
		colorsArr[index] = color;
	}
	this->setGeometryEntitiesColorArray(colorsArr);
	this->endGroup();

	/* ================================================================
	Rendering
	===================================================================*/
	this->beginGroup(SettingsRoots::rendering);
	AppDefaultColors::RendererColorsArray renColorsArr;

	for (size_t index = 0; index < renColorsArr.size(); ++index) {
		QString colorVar = QString::fromStdString("color" + std::to_string(index));
		QColor color = this->value(colorVar).value<QColor>();
		renColorsArr[index] = color;
	}
	this->setRendererColorsArray(renColorsArr);
	this->setGradientBackgroundEnabled(
		this->value("GradientBackgroundEnabled").toBool());
	this->setRendererGradientMode(
		static_cast<vtkRenderer::GradientModes>(
			this->value("GradientBackgroundMode").toInt()));
	this->endGroup();
}

//--------------------------------------------------------------------------------------
void AppSettings::updateRendererSettings() {
	this->beginGroup(SettingsRoots::rendering);

	this->setValue(
		"GradientBackgroundEnabled",
		this->isGradientBackgroundEnabled());
	this->setValue(
		"GradientBackgroundMode",
		static_cast<int>(this->getRendererGradientMode()));

	AppDefaultColors::RendererColorsArray colorsArr
		= this->getRendererColorsArray();

	for (size_t index = 0; index < colorsArr.size(); ++index) {
		QString colorVar = QString::fromStdString("color" + std::to_string(index));
		this->setValue(colorVar, colorsArr[index]);
	}

	this->endGroup();
}