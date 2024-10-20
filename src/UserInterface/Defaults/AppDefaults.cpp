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

const QString AppDefaults::_subItemsSetupPath  = ":templates/templates/SubItemsSetup.json";
const QString AppDefaults::_rootItemsSetupPath = ":templates/templates/RootItemsSetup.json";
const QString AppDefaults::_comboBoxModelsPath = ":templates/templates/ComboboxModels.json";

//--------------------------------------------------------------------------------------
AppDefaults::AppDefaults()
	: settings("settingsFile.ini", QSettings::IniFormat)
	, _appDefaultColors(AppDefaultColors()) {

	settings.beginGroup("ApplicationDefaults");
	settings.setValue("Version", "1.0.0");
	settings.setValue("ProjFileVersion", "1.0");
	settings.setValue("Name", "MeshGeneratingTool");
	// Space for more settings
	settings.endGroup();

	// Read JSON file representing combobox models
	QString filePath = this->getComboBoxModelsPath();
	this->readJSONFile(m_comboBoxModels, filePath);
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
	return &m_comboBoxModels;
}

//--------------------------------------------------------------------------------------
QString AppDefaults::getValue(const QString& key) {
	this->settings.beginGroup("ApplicationDefaults");
	QString value = this->settings.value(key).toString();
	this->settings.endGroup();
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
const QString AppDefaults::getRootItemsSetupPath() {
	return this->_rootItemsSetupPath;
}

//--------------------------------------------------------------------------------------
const QString AppDefaults::getComboBoxModelsPath() {
	return this->_comboBoxModelsPath;
}

const QString AppDefaults::getSubItemsSetupPath(){
	return this->_subItemsSetupPath;
};

//--------------------------------------------------------------------------------------
const AppDefaultColors::GeomColorsArray AppDefaults::getGeometryEntitiesColorArray() {
	return _appDefaultColors.getGeometryEntitiesColorArray();
}
