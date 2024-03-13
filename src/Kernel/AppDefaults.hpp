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

#include <QSettings>

/**
 * @brief  Class for handling default application properties like name, version, etc.
 *
 */
class AppDefaults {
public:
	AppDefaults() {
		settings.beginGroup("ApplicationDefaults");
		settings.setValue("Version", "1.0.0");
		settings.setValue("ProjFileVersion", "1.0");
		settings.setValue("Name", "MeshGeneratingTool");
		// Space for more settings
		settings.endGroup();
	}
	~AppDefaults() = default;

	/**
	 * @brief  Get name of application as a QString.
	 *
	 * @return {QString}  : Name of application
	 */
	QString getAppName() {
		return getValue("Name");
	}

	/**
	 * @brief  Get version of application as a QString.
	 *
	 * @return {QString}  : Version of application.
	 */
	QString getAppVersion() {
		return getValue("Version");
	}

	/**
	 * @brief  Get version of application proejct file as a QString.
	 *
	 * @return {QString}  : Version of application project file.
	 */
	QString getAppProjFileVersion() {
		return getValue("ProjFileVersion");
	}

private:
	// Container for application settings
	QSettings settings;

	/**
	 * @brief  Get application default setting for key with given name.
	 *
	 * @param  {QString} key : Key representing default setting.
	 * @return {QString}     : Corresponding value for given settings key.
	 */
	QString getValue(const QString& key) {
		settings.beginGroup("ApplicationDefaults");
		QString value = settings.value(key).toString();
		settings.endGroup();
		return value;
	}
};