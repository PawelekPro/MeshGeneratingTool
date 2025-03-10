/*
 * Copyright (C) 2024 Paweł Gilewicz
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

#include "AppInfo.hpp"

QString AppInfo::appName() { return "MeshGeneratingTool"; }
QString AppInfo::appVersion() { return "1.0.0"; }
QString AppInfo::appProjFileVersion() { return "1.0"; }

const QString AppInfo::subItemsSetupPath
	= QStringLiteral(":templates/templates/SubItemsSetup.json");
const QString AppInfo::rootItemsSetupPath
	= QStringLiteral(":templates/templates/RootItemsSetup.json");
const QString AppInfo::comboBoxModelsPath
	= QStringLiteral(":templates/templates/ComboboxModels.json");
const QString AppInfo::visibilityRulesPath
	= QStringLiteral(":templates/templates/DisplayRules.xml");

//-----------------------------------------------------------------------------
AppInfo::AppInfo()
	: settings("settingsFile.ini", QSettings::IniFormat) {

	settings.beginGroup("ApplicationDefaults");
	settings.setValue("Version", "1.0.0");
	settings.setValue("ProjFileVersion", "1.0");
	settings.setValue("Name", "MeshGeneratingTool");
	settings.endGroup();
}

//-----------------------------------------------------------------------------
QString AppInfo::getValue(const QString& key) {
	this->settings.beginGroup("ApplicationDefaults");
	QString value = this->settings.value(key).toString();
	this->settings.endGroup();
	return value;
}

//-----------------------------------------------------------------------------
QString AppInfo::getAppVersion() { return this->getValue("Version"); }

//-----------------------------------------------------------------------------
QString AppInfo::getAppName() { return this->getValue("Name"); }

//-----------------------------------------------------------------------------
QString AppInfo::getAppProjFileVersion() {
	return this->getValue("ProjFileVersion");
}

//-----------------------------------------------------------------------------
QString AppInfo::getRootItemsSetupPath() const {
	return this->rootItemsSetupPath;
}

//-----------------------------------------------------------------------------
QString AppInfo::getComboBoxModelsPath() const {
	return this->comboBoxModelsPath;
}

//-----------------------------------------------------------------------------
QString AppInfo::getSubItemsSetupPath() const {
	return this->subItemsSetupPath;
};

QString AppInfo::getVisibilityRulesPath() const {
	return this->visibilityRulesPath;
}