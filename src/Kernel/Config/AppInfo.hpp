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

#ifndef APPINFO_HPP
#define APPINFO_HPP

#include <QFile>
#include <QSettings>

#include <filesystem>
#include <iostream>

/**
 * @brief The AppInfo class provides access to application-wide default settings.
 *
 * This class is designed to hold default settings and configurations that are used
 * throughout the application. It follows the Singleton design pattern to ensure that
 * only one instance of the class is created and provides a global point of access
 * to that instance through the static method getInstance().
 */
class AppInfo {
public:
	/**
	 * @brief Provides a global point of access to the singleton instance of the AppInfo class.
	 *
	 * This static member function follows the Singleton design pattern to ensure that only one instance
	 * of the AppInfo class is created throughout the application's lifetime. It returns a reference
	 * to the singleton instance. If the instance does not exist yet, it creates one. Subsequent calls
	 * to this function return the same instance.
	 *
	 * @return A reference to the singleton instance of the AppInfo class.
	 */
	static AppInfo& getInstance() {
		static AppInfo instance;
		return instance;
	};

	static const QString appName();
	static const QString appVersion();
	static const QString appProjFileVersion();

	/**
	 * @brief  Get name of application as a QString.
	 *
	 * @return {QString}  : Name of application
	 */
	QString getAppName();

	/**
	 * @brief  Get version of application as a QString.
	 *
	 * @return {QString}  : Version of application.
	 */
	QString getAppVersion();

	/**
	 * @brief  Get version of application project file as a QString.
	 *
	 * @return {QString}  : Version of application project file.
	 */
	QString getAppProjFileVersion();

	/**
	 * @brief Returns the path to the templates directory.
	 *
	 * The templates directory contains predefined templates used by the application.
	 *
	 * @return The path to the templates directory as a QString.
	 */
	const QString getRootItemsSetupPath();

	/**
	 * @brief Returns the path to the templates directory.
	 *
	 * The templates directory contains predefined templates used by the application.
	 *
	 * @return The path to the templates directory as a QString.
	 */
	const QString getSubItemsSetupPath();

	/**
	 * @brief Returns the path to the ComboBox models directory.
	 *
	 * The ComboBox models directory contains predefined models used for ComboBoxes
	 * within the application.
	 *
	 * @return The path to the ComboBox models directory as a QString.
	 */
	const QString getComboBoxModelsPath();

private:
	AppInfo(AppInfo const&) = delete;
	AppInfo();
	~AppInfo() = default;
	void operator=(AppInfo const&) = delete;

	/**
	 * @brief  Get application default setting for key with given name.
	 *
	 * @param  {QString} key : Key representing default setting.
	 * @return {QString}     : Corresponding value for given settings key.
	 */
	QString getValue(const QString& key);

private:
	// Container for application settings
	QSettings settings;

	// Templates path
	static const QString _subItemsSetupPath;
	static const QString _rootItemsSetupPath;
	static const QString _comboBoxModelsPath;
};

#endif