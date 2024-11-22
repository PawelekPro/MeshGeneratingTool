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

#include "AppTheme.h"
#include "Configuration.hpp"
#include "DocumentHandler.hpp"
#include "Model.hpp"
#include "PreferencesDialog.h"
#include "ProgressBar.hpp"
#include "QVTKRenderWindow.h"
#include "SARibbon.h"
#include "TreeStructure.hpp"
#include <QmlStyleUrlInterceptor.h>

#include <QApplication>
#include <QButtonGroup>
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
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

	std::shared_ptr<Model> model;

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
	 * @brief  Create an empty model.
	 */
	void newModel();

	/**
	 * @brief  Generate mesh based on current settings.
	 */
	void generateMesh();

	/**
	 * @brief  Toggle mesh view on -> add Model mesh actor to renderer
	 */
	void showMesh();

	/**
	 * @brief  When mesh view is disabled -> add Model geometry actor to renderer
	 */
	void showGeometry();

	/**
	 * @brief  Get access to progress bar.
	 *
	 * @return {ProgressBar*}  : Progress bar instance.
	 */
	ProgressBar* getProgressBar() const { return this->progressBar; }

	// Progress bar widget
	ProgressBar* progressBar;

public:
	Rendering::QVTKRenderWindow* getRenderWindow() {
		return this->QVTKRender;
	}

private:
	Ui::MainWindow* ui;

	// Rendering window widget
	Rendering::QVTKRenderWindow* QVTKRender;

	/**
	 * @brief Connect widgets to the methods.
	 *
	 */
	void setConnections();

	// Push button group of the switch type
	QButtonGroup buttonGroup;

	/**
	 * @brief  Set the initial state of the actions, ex. grey out imports unitl
	 * model is created
	 */
	void initializeActions();

private slots:
	/**
	 * @brief  Handle changing entity (edges/faces/etc) selection event.
	 *
	 * @param  {QAbstractButton*} button : Button that sends a signal
	 */
	void handleSelectorButtonClicked(QAbstractButton* button);

	/**
	 * This function is called when an item selection of tree structure is changed.
	 *
	 * @param None
	 *
	 * @returns None
	 */
	void onItemSelectionChanged();

public slots:
	void onShowDialogButtonClicked();
};
#endif // MAINWINDOW_H
