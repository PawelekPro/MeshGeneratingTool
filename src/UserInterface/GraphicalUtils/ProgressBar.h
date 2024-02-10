/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 *
 * Created by Paweł Gilewicz on 01/02/2024.
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

// Take a look:
// https://opencascade.blogspot.com/2009/01/

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "./ui_ProgressBar.h"
#include <iostream>

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
		show();
		setValue(0);
	}

	void finish();

	void setProgressMessage(const std::string text);

private:
	Ui::ProgressBar* ui;
};

#endif
