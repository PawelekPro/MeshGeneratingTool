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

#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

/**
 * @brief A base class for custom QWidget-based widgets.
 *
 * The BaseWidget class serves as a base class for custom widgets and defines a common interface
 * for setting the QModelIndex associated with the widget.
 *
 * This class is intended to be subclassed to create specialized widgets with specific functionality.
 *
 * @note This class inherits from QWidget and defines a pure virtual function setIndex(),
 *       which must be implemented by subclasses to set the QModelIndex associated with the widget.
 */
class BaseWidget : public QWidget {
	Q_OBJECT
public:
	BaseWidget(QWidget* parent = nullptr)
		: QWidget(parent) {};
	virtual ~BaseWidget() {};
	virtual void setIndex(const QModelIndex& index) = 0;
};

#endif