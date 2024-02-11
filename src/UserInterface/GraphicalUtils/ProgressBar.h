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

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "ui_ProgressBar.h"
#include <iostream>

#include <vtkLogger.h>

#include <QEvent>
#include <QPushButton>
#include <QWidget>
#include <QtCore/QtGlobal>

QT_BEGIN_NAMESPACE
namespace Ui {
class ProgressBar;
}
QT_END_NAMESPACE

class ProgressBar : public QWidget {
	Q_OBJECT

public:
	ProgressBar(QWidget* parent = nullptr);
	~ProgressBar();

	/**
	 * @brief  Set currently displayed value of progress bar.
	 *
	 * @param  {int} value : Progress value of current process.
	 */
	void setValue(const int value);

	void initialize() {
		this->show();
		this->ui->stopButton->show(); // fix for button not being displayed
		setValue(0);
	}

	/**
	 * @brief  Set maximum value of progress bar.
	 *
	 * @param  {int} max : Maximum value.
	 */
	void setMaximum(const int max);

	/**
	 * @brief  Set minimum value of progress bar.
	 *
	 * @param  {int} min : Minimum value.
	 */
	void setMinimum(const int min);

	/**
	 * @brief  Finish progress indication and hide progress bar.
	 */
	void finish();

	/**
	 * @brief  Set currently displayed message on progress bar.
	 *
	 * @param  {std::string} text : Text to be displayed.
	 */
	void setProgressMessage(const std::string text);

	/**
	 * @brief  Get terminate signal (bool indicator).
	 *
	 * @return {bool}  : True if process aborted, false otherwise.
	 */
	bool getTerminateIndicator();

	/**
	 * @brief  Set terminate signal indicator.
	 *
	 * @param  {bool} sigTerm : True if terminated, false otherwise.
	 */
	void setTerminateIndicator(const bool);

private:
	// interface
	Ui::ProgressBar* ui;

	// Terminate signal (bool indicator)
	bool _terminate = false;

private slots:
	/**
	 * @brief Slot for handling stop button pressed event.
	 *
	 */
	void handleStopButtonClicked();
};

#endif
