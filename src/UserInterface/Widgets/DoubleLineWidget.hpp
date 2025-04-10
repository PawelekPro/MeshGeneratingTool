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

#ifndef DOUBLELINEWIDGET_HPP
#define DOUBLELINEWIDGET_HPP

#include "AbstractLineEdit.h"

#include <QDoubleValidator>
#include <QModelIndex>
#include <QWidget>

/**
 * @brief A custom widget with two QLineEdit fields for representing a double value.
 *
 * The DoubleLineWidget class extends the functionality of the AbstractLineEdit widget
 * to provide a specialized widget for representing double values with two QLineEdit fields.
 *
 * This class is intended to be used for displaying and editing double values in a graphical user interface.
 *
 * @note This class inherits from AbstractLineEdit and adds functionality specific to representing double values.
 *
 * @see AbstractLineEdit
 */
class DoubleLineWidget : public AbstractLineEdit {
	Q_OBJECT
public:
	/**
	 * @brief Constructs a DoubleLineWidget object.
	 *
	 * @param parent The parent widget to which this widget belongs.
	 */
	DoubleLineWidget(QWidget* parent = nullptr);

	/**
	 * @brief Destroys the DoubleLineWidget object.
	 */
	~DoubleLineWidget() override = default;

	/**
	 * @brief Sets the QModelIndex associated with the DoubleLineWidget and initializes its data.
	 *
	 * This function sets the QModelIndex associated with the DoubleLineWidget and initializes its data
	 * based on the data retrieved from the specified QModelIndex. It also establishes connections
	 * for value change signals and slots.
	 *
	 * @param index The QModelIndex from which to initialize the widget and its data.
	 *
	 * @note This function assumes that the AbstractLineEdit contains necessary initialization logic
	 *       and that the model associated with the index provides valid data.
	 *       If the data is missing or malformed, the behavior is undefined.
	 */
	void setIndex(const QModelIndex& index) override;

private:
	QModelIndex _index; /**< The QModelIndex associated with the DoubleLineWidget. */

public slots:
	 /*
	 * @brief Slot function invoked when the value of the DoubleLineWidget changes.
	 *
	 * This function is called when the value of the DoubleLineWidget changes.
	 * It updates the data in the model associated with the widget's QModelIndex with the new value.
	 *
	 * @param value The new value of the DoubleLineWidget as a QString.
	 *
	 * @note This function assumes that the model associated with the index allows modifying data
	 *       and that the index itself is valid.
	 *       If the model or index is invalid, the behavior is undefined.
	 */
	void onValueChanged(QString);
};

#endif