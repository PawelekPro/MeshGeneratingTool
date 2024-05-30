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

#include "AppDefaults.h"

const QString AppDefaults::_templatesPath = ":/templates/templates/ProjectSetup.json";
const QString AppDefaults::_comboBoxModelsPath = ":/templates/templates/ComboboxModels.json";

//--------------------------------------------------------------------------------------
AppDefaults::AppDefaults()
	: _settings(AppSettings())
	, _appDefaultColors(AppDefaultColors()) {

	// Read JSON file representing combobox models
	QString filePath = this->getComboBoxModelsPath();
	this->readJSONFile(_comboBoxModels, filePath);
}

//--------------------------------------------------------------------------------------
void AppDefaults::readJSONFile(rapidjson::Document& doc, QString filePath) {
	QFile jsonFile(filePath);

	if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		vtkLogF(ERROR, "Failed to open ComboboxModels.json file.");
	}

	QByteArray jsonData = jsonFile.readAll();
	jsonFile.close();

	// Parse the JSON string
	doc.Parse(jsonData.constData());

	if (doc.HasParseError()) {
		vtkLogF(ERROR, ("Error parsing JSON file: " + filePath.toStdString()).c_str());
	}
}

//--------------------------------------------------------------------------------------
rapidjson::Document* AppDefaults::getComboBoxModels() {
	return &_comboBoxModels;
}

//--------------------------------------------------------------------------------------
QString AppDefaults::getValue(const QString& key) {
	this->_settings.beginGroup(SettingsRoots::defaults);
	QString value = this->_settings.value(key).toString();
	this->_settings.endGroup();
	return value;
}

//--------------------------------------------------------------------------------------
QString AppDefaults::getAppVersion() {
	return this->getValue("Version");
}

//--------------------------------------------------------------------------------------
QString AppDefaults::getAppName() {
	return this->getValue("Name");
}

//--------------------------------------------------------------------------------------
QString AppDefaults::getAppProjFileVersion() {
	return this->getValue("ProjFileVersion");
}

//--------------------------------------------------------------------------------------
const QString AppDefaults::getTemplatesPath() {
	return this->_templatesPath;
}

//--------------------------------------------------------------------------------------
const QString AppDefaults::getComboBoxModelsPath() {
	return this->_comboBoxModelsPath;
}

//--------------------------------------------------------------------------------------
const AppDefaultColors::GeomColorsArray AppDefaults::getGeometryEntitiesColorArray() {
	return _appDefaultColors.getGeometryEntitiesColorArray();
}