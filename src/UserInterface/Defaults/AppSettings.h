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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "AppDefaultColors.h"
#include "AppInfo.h"
#include "AppTheme.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>

namespace SettingsRoots {
const std::string defaults = "Defaults";
const std::string rendering = "Rendering";
const std::string graphics = "Graphics";
const std::string theme = "Theme";
};

class AppSettings : public QSettings, public AppDefaultColors {
	Q_OBJECT
public:
	explicit AppSettings();
	~AppSettings() = default;

public:
	void updateRendererSettings();
	const QString getThemeAsString(); 
	void setThemeString(QString theme);

private:
	void createDefaultSettings();
	void loadDefaultSettings();
};

#endif