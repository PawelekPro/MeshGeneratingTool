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

#include "AppInfo.h"
#include "MainWindow.hpp"
#include "ModelInterface"
#include "ModelLifetCycleManager.hpp"

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QVTKOpenGLNativeWidget.h>
#include <memory>

int main(int argc, char* argv[]) {
	// Before initializing QApplication, set the default surface format
	// (https://vtk.org/doc/nightly/html/classQVTKOpenGLNativeWidget.html#details)
	auto format = QVTKOpenGLNativeWidget::defaultFormat();
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(format);

	QCoreApplication::setOrganizationName(AppInfo::appName);
	QCoreApplication::setApplicationName(AppInfo::appName);

	QApplication application(argc, argv);

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString& locale : uiLanguages) {
		const QString baseName = "meshGeneratorUI_" + QLocale(locale).name();
		if (translator.load(":/i18n/" + baseName)) {
			application.installTranslator(&translator);
			break;
		}
	}

	ModelLifecycleManager modelManager;
	modelManager.createNewModel("NewModel");
	std::shared_ptr<ModelInterface> = std::make_shared<ModelInterface>(modelManager);
	MainWindow window(modelInterface);
	window.showMaximized();
	return application.exec();
}
