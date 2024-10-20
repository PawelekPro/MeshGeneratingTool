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

#ifndef APPDEFAULTS_H
#define APPDEFAULTS_H

#include "AppDefaultColors.h"

#include <QFile>
#include <QSettings>

#include <filesystem>
#include <iostream>

#include <vtkLogger.h>

#include "document.h"
#include "filereadstream.h"

namespace fs = std::filesystem;

/**
 * @brief The AppDefaults class provides access to application-wide default settings.
 *
 * This class is designed to hold default settings and configurations that are used
 * throughout the application. It follows the Singleton design pattern to ensure that
 * only one instance of the class is created and provides a global point of access
 * to that instance through the static method getInstance().
 */
class AppDefaults {
public:
	/**
	 * @brief Provides a global point of access to the singleton instance of the AppDefaults class.
	 *
	 * This static member function follows the Singleton design pattern to ensure that only one instance
	 * of the AppDefaults class is created throughout the application's lifetime. It returns a reference
	 * to the singleton instance. If the instance does not exist yet, it creates one. Subsequent calls
	 * to this function return the same instance.
	 *
	 * @return A reference to the singleton instance of the AppDefaults class.
	 */
	static AppDefaults& getInstance() {
		static AppDefaults instance;
		return instance;
	};

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
	 * @brief Returns a pointer to the ComboBox models stored in the AppDefaults class.
	 *
	 * This function provides access to the ComboBox models stored within the AppDefaults class.
	 * It returns a pointer to a rapidjson::Document object containing the ComboBox models.
	 * This allows external code to access and manipulate the models as needed.
	 *
	 * @return A pointer to the rapidjson::Document object containing the ComboBox models.
	 */
	rapidjson::Document* getComboBoxModels();

	const AppDefaultColors::GeomColorsArray getGeometryEntitiesColorArray();

private:
	AppDefaults(AppDefaults const&) = delete;
	AppDefaults();
	~AppDefaults() = default;
	void operator=(AppDefaults const&) = delete;

	/**
	 * @brief  Get application default setting for key with given name.
	 *
	 * @param  {QString} key : Key representing default setting.
	 * @return {QString}     : Corresponding value for given settings key.
	 */
	QString getValue(const QString& key);

	/**
	 * @brief Reads and parses a JSON file.
	 *
	 * This function reads the content of a JSON file specified by the filePath parameter
	 * and parses it into the provided rapidjson::Document object, doc. It opens the file,
	 * reads its content, closes the file, and then parses the JSON data into the document.
	 * If any errors occur during the process, it logs an error message.
	 *
	 * @param doc A reference to a rapidjson::Document object where the parsed JSON data will be stored.
	 * @param filePath The path to the JSON file to be read and parsed.
	 */
	void readJSONFile(rapidjson::Document&, QString);

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
	// Container for application settings
	QSettings settings;

	// Templates path
	static const QString _subItemsSetupPath;
	static const QString _rootItemsSetupPath;
	static const QString _comboBoxModelsPath;

	// rapidjson document for ComboBoxModels JSON file
	rapidjson::Document m_comboBoxModels;

	AppDefaultColors _appDefaultColors;
};

#endif