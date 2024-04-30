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

#include <QFile>
#include <QSettings>

#include <filesystem>
#include <iostream>

#include <vtkLogger.h>

#include "document.h"
#include "filereadstream.h"

namespace fs = std::filesystem;

/**
 * @brief  Class for handling default application properties like name, version, etc.
 *
 */
class AppDefaults {
public:
	AppDefaults();
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

	const QString getTemplatesPath() {
		return this->_templatesPath;
	}

	const QString getComboBoxModelsPath() {
		return this->_comboBoxModelsPath;
	}

	void readJSONFile(QString);

	static AppDefaults& getInstance() {
		static AppDefaults instance;
		return instance;
	}

	rapidjson::Document* getDocument() {
		return &m_document;
	}

private:
	AppDefaults(AppDefaults const&) = delete;
	void operator=(AppDefaults const&) = delete;

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

	// Container for application settings
	QSettings settings;

	// Templates path
	static const QString _templatesPath;
	static const QString _comboBoxModelsPath;

	// rapidjson document
	rapidjson::Document m_document;
};

#endif