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

#include "MainWindow.hpp"
#include "ModelInterface.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QVTKOpenGLNativeWidget.h>
#include <memory>

// logging
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
	spdlog::set_level(spdlog::level::trace);
	spdlog::debug("Starting application");

	// Before initializing QApplication, set the default surface format
	// (https://vtk.org/doc/nightly/html/classQVTKOpenGLNativeWidget.html#details)
	auto format = QVTKOpenGLNativeWidget::defaultFormat();
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(format);

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

	ModelManager modelManager;
	modelManager.createNewModel("NewModel");
	std::shared_ptr<ModelInterface> modelInterface = std::make_shared<ModelInterface>(modelManager);
	MainWindow window(modelInterface);
	window.showMaximized();
	return application.exec();
}
