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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <functional>

#include "../CADManager/STLFileReader.h"
#include "../GeometryCore/STEPPlugin/STEPPluginOperations.h"
#include "./ui_mainwindow.h"
#include "GraphicalUtils/Configuration.h"
#include "GraphicalUtils/ProgressBar.h"
#include "Rendering/QVTKRenderWindow.h"

#include <vtkAxesActor.h>

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>

// Declaration of callable type
using Callable = std::function<void(QString)>;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	/**
	 * @brief  Open system dialog for selecting file and perform defined action with selected file.
	 *
	 * @param  {Callable} action    : Action to be performed.
	 * @param  {QString} actionName : Action name to be displayed in dialog box.
	 * @param  {QString} filter     : Filter of files format that can be selected.
	 * @return {int}                : Accepted code if worked, Rejected code otherwise.
	 */
	int openFileDialog(Callable action, QString actionName, QString filter);

	/**
	 * @brief  Import step file, load it to project DOM and render geometry.
	 *
	 * @param  {QString} fileName : Path to the physical STEP file.
	 */
	void importSTEP(QString fileName);

	/**
	 * @brief  Import stl file, load it to project DOM and render geometry.
	 *
	 * @param  {QString} fileName : Path to the STL file.
	 */
	void importSTL(QString fileName);

	/**
	 * @brief  Get access to progress bar.
	 *
	 * @return {ProgressBar*}  : Progress bar instance.
	 */
	ProgressBar* getProgressBar() const { return this->progressBar; }

	// Progress bar
	ProgressBar* progressBar;

private:
	Ui::MainWindow* ui;

	// Render window
	Rendering::QVTKRenderWindow* QVTKRender;

	/**
	 * @brief Connect widgets to the methods.
	 *
	 */
	void setConnections();
};
#endif // MAINWINDOW_H
