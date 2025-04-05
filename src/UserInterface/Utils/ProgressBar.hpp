/*
 * Copyright (C) 2024 Paweł Gilewicz
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

#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include <iostream>

#include <QPushButton>
#include <QWidget>
#include <QtCore/QtGlobal>

QT_BEGIN_NAMESPACE
namespace Ui {
class ProgressBar;
}
QT_END_NAMESPACE

class ProgressBar final : public QWidget {
	Q_OBJECT

public:
	explicit ProgressBar(QWidget* parent = nullptr);
	~ProgressBar() override;

	/**
	 * @brief  Set currently displayed value of progress bar.
	 *
	 * @param  {int} value : Progress value of current process.
	 */
	void setValue(const int value) const;

	/**
	 * @brief  Set up the initial state of the widget and display it..
	 *
	 */
	void initialize();

	/**
	 * @brief  Set maximum value of progress bar.
	 *
	 * @param  {int} max : Maximum value.
	 */
	void setMaximum(const int max) const;

	/**
	 * @brief  Set minimum value of progress bar.
	 *
	 * @param  {int} min : Minimum value.
	 */
	void setMinimum(const int min) const;

	/**
	 * @brief  Finish progress indication and hide progress bar.
	 */
	void finish();

	/**
	 * @brief  Set currently displayed message on progress bar.
	 *
	 * @param  {std::string} text : Text to be displayed.
	 */
	void setProgressMessage(const std::string& text) const;

	/**
	 * @brief  Get terminate signal (bool indicator).
	 *
	 * @return {bool}  : True if process aborted, false otherwise.
	 */
	bool getTerminateIndicator() const;

	/**
	 * @brief  Set terminate signal indicator.
	 *
	 * @param  {bool} sigTerm : True if terminated, false otherwise.
	 */
	void setTerminateIndicator(const bool);

signals:
	void terminateProcess();

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
