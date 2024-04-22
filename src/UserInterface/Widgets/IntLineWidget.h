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

#ifndef INTLINEEDIT_H
#define INTLINEEDIT_H
#pragma once

#include "AbstractLineEdit.h"

#include <QIntValidator>
#include <QModelIndex>
#include <QObject>
#include <QWidget>

class IntLineWidget : public AbstractLineEdit {
	Q_OBJECT
public:
	IntLineWidget(QWidget* parent = nullptr);
	~IntLineWidget() {};

	void setIndex(const QModelIndex&) override;

private:
	QModelIndex m_index;

signals:
	void valueChanged(int);

public slots:
	void onValueChanged(const int);
};

#endif