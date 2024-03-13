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

	QString getAppName() {
		settings.beginGroup("ApplicationDefaults");
		QString name = settings.value("Name").toString();
		settings.endGroup();
		return name;
	}

	QString getAppVersion() {
		settings.beginGroup("ApplicationDefaults");
		QString version = settings.value("Version").toString();
		settings.endGroup();
		return version;
	}

	QString getAppProjFileVersion() {
		settings.beginGroup("ApplicationDefaults");
		QString projFileVersion = settings.value("ProjFileVersion").toString();
		settings.endGroup();
		return projFileVersion;
	}

private:
	// Container for application settings
	QSettings settings;
};