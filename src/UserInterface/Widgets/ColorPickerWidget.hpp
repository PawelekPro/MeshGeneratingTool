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

#ifndef COLORPICKERWIDGET_HPP
#define COLORPICKERWIDGET_HPP

#include "BaseWidget.hpp"

#include <QAbstractItemModel>
#include <QColor>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QPushButton>
#include <QVariant>

#include <tuple>

class ColorPickerWidget : public BaseWidget {
	Q_OBJECT
public:
	/**
	 * @brief Construct an ColorPickerWidget object.
	 *
	 * @param parent The parent widget to which this widget belongs.
	 */
	explicit ColorPickerWidget(QWidget* parent = nullptr);

	/**
	 * @brief Destroy the ColorPickerWidget object.
	 */
	~ColorPickerWidget();

	void setIndex(const QModelIndex& index) override;
	void setValue(const std::tuple<int, int, int>& color);

private:
	QPushButton* _selectionButton;
	QModelIndex _index;

private slots:
	void onSelectColorClicked();
};

#endif