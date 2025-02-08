/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <functional>

#include "Configuration.hpp"

#include "ModelActionsHandler.hpp"
#include "ModelInterface.hpp"
#include "RenderSignalSender.hpp"

#include "AppTheme.hpp"
#include "DocumentHandler.hpp"
#include "PreferencesDialog.h"
#include "ProgressBar.hpp"
#include "QVTKRenderWindow.hpp"
#include "RenderSignalHandler.hpp"
#include "SARibbon.h"
#include "TreeStructure.hpp"

#include "FileDialogUtils.hpp"

#include <QButtonGroup>
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(std::shared_ptr<ModelInterface> aModelInterface, QWidget* parent = nullptr);
	~MainWindow();

	/**
	 * @brief  Get access to progress bar.
	 *
	 * @return {ProgressBar*}  : Progress bar instance.
	 */
	ProgressBar* getProgressBar() const { return this->progressBar; }

public:
	Rendering::QVTKRenderWindow* getRenderWindow() {
		return this->QVTKRender;
	}

private:
	Ui::MainWindow* ui;
	std::shared_ptr<ModelInterface> _modelInterface;

	// Rendering window widget
	Rendering::QVTKRenderWindow* QVTKRender;
	Rendering::RenderSignalHandler* _renderSignalHandler;
	RenderSignalSender* _renderSignalSender;
	ModelActionsHandler* _modelHandler;

	// Progress bar widget
	ProgressBar* progressBar;

	/**
	 * @brief connects actions defined in mainwindow to modelhandler.
	 *
	 */
	void connectActionsToModel();

	/**
	 * @brief connects actions defined in mainwindow to modelhandler.
	 *
	 */
	void setupModelObservers();


	// Push button group of the switch type
	QButtonGroup buttonGroup;

	/**
	 * @brief  Connects signals from RenderSignalSender to RenderSignalHandler. This allows to include RenderWindow
	 * only in mainwindow and pass only the signalSenders down the UI.
	 */
	void connectModelToRenderWindow(RenderSignalSender* aSignalSender, Rendering::RenderSignalHandler* aSignalHandler);

private slots:
	/**
	 * @brief  Handle changing entity (edges/faces/etc) selection event.
	 *
	 * @param  {QAbstractButton*} button : Button that sends a signal
	 */
	void handleSelectorButtonClicked(QAbstractButton* button);

	/**
	 *  @brief This function is called when an item selection of tree structure is changed.
	 */
	void onItemSelectionChanged();

public slots:
	void onShowDialogButtonClicked();
};
#endif
