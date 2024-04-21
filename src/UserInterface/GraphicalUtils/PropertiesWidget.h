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

#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include "PropertiesModel.h"

#include <QList>
#include <QTableView>
#include <QWidget>

/**
 * Custom widget for displaying properties using a table view.
 */
class PropertiesWidget : public QTableView {
	Q_OBJECT
public:
	explicit PropertiesWidget(QWidget* parent = nullptr)
		: QTableView(parent) {};
	~PropertiesWidget();

	/**
	 * Sets the PropertiesModel for the object.
	 *
	 * @param model A pointer to the PropertiesModel to be set.
	 *
	 * @returns None
	 */
	void setModel(PropertiesModel*);

private:
	// Container for temporary widgets existing between model selection change event
	QList<QWidget*> m_createdWidgets;
};

#endif