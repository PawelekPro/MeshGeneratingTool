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
AppDefaults::AppDefaults() {
	settings.beginGroup("ApplicationDefaults");
	settings.setValue("Version", "1.0.0");
	settings.setValue("ProjFileVersion", "1.0");
	settings.setValue("Name", "MeshGeneratingTool");
	// Space for more settings
	settings.endGroup();

	// Read JSON file representing combobox models
	QString filePath = this->appInfo.getComboBoxModelsPath();
	this->readJSONFile(filePath);
}

//--------------------------------------------------------------------------------------
void AppDefaults::readJSONFile(QString filePath){
	// QString filePath = this->appInfo.getComboBoxModelsPath();
	QFile jsonFile(filePath);

	if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		vtkLogF(ERROR, "Failed to open ComboboxModels.json file.");
	}

	QByteArray jsonData = jsonFile.readAll();
	jsonFile.close();

	// Parse the JSON string
	this->m_document.Parse(jsonData.constData());

	if (this->m_document.HasParseError()) {
		vtkLogF(ERROR, "Error parsing JSON file.");
	}
}