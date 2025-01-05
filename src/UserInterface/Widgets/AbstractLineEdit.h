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

#ifndef ABSTRACTLINEEDIT_H
#define ABSTRACTLINEEDIT_H

#include "BaseWidget.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QModelIndex>
#include <QValidator>
#include <QWidget>
#include <QXmlStreamAttributes>

/**
 * @brief A custom QLineEdit-based widget with extended functionality.
 *
 * The AbstractLineEdit class extends the functionality of the QLineEdit widget
 * to provide additional features such as initialization from a model, setting and retrieving values,
 * and validation of input.
 *
 * This class is intended to be used as a base class for more specialized line edit widgets.
 *
 * @note This class inherits from BaseWidget and extends it with QLineEdit-specific functionality.
 *
 * @see BaseWidget
 */
class AbstractLineEdit : public BaseWidget {
	Q_OBJECT

public:
	AbstractLineEdit(QWidget* parent = nullptr, QValidator* validator = nullptr);
	~AbstractLineEdit();

	/**
	 * @brief Initializes the AbstractLineEdit widget with data from the model.
	 *
	 * This function initializes the AbstractLineEdit widget using data retrieved from the specified QModelIndex.
	 * It expects the model associated with the index to be of type PropertiesModel.
	 *
	 * @param index The QModelIndex from which to initialize the widget.
	 * @throws std::invalid_argument if the model assigned to the index is not of the 'PropertiesModel' type.
	 *
	 * The function retrieves the QDomElement associated with the given index from the PropertiesModel.
	 * It then extracts attributes such as 'unit', 'minVal', and 'maxVal' from the QDomElement
	 * and assigns them to corresponding properties of the AbstractLineEdit widget.
	 *
	 * @note This function assumes that the model contains properties such as 'unit', 'minVal', and 'maxVal'
	 *       associated with the QModelIndex. If the model or the properties are missing or malformed,
	 *       the behavior is undefined.
	 */
	void initialize(const QModelIndex& index);

	/**
	 * @brief Sets the value of the AbstractLineEdit widget.
	 *
	 * This function sets the text value of the QLineEdit sub-widget of the AbstractLineEdit
	 * to the specified string value.
	 *
	 * @param value The string value to set as the text of the QLineEdit.
	 *
	 * @note This function assumes that the AbstractLineEdit contains a QLineEdit sub-widget (m_lineEdit).
	 *       If m_lineEdit is not initialized or nullptr, the behavior is undefined.
	 */
	void setValue(const std::string&);

	/**
	 * @brief Retrieves the current value of the AbstractLineEdit widget.
	 *
	 * This function returns the text value of the QLineEdit sub-widget of the AbstractLineEdit
	 * as a standard string.
	 *
	 * @return The current value of the AbstractLineEdit widget as a string.
	 *
	 * @note This function assumes that the AbstractLineEdit contains a QLineEdit sub-widget (m_lineEdit).
	 *       If m_lineEdit is not initialized or nullptr, the behavior is undefined.
	 */
	std::string getValue();

	/**
	 * @brief Defines constants for line height and font size.
	 *
	 * @note These constants are intended to be used as read-only values and should not be modified
	 *       during the runtime of the program.
	 */
	static const int lineHeight = 20;
	static const int fontSize = 10;

protected:
	QLineEdit* _lineEdit;
	QLabel* _suffixLabel;
	double _lastValidValue;
	double _minVal, _maxVal;

	QModelIndex _index;
	QLocale _locale;

	static QString labelStyleSheet;
	static QString lineEditStyleSheet;

signals:
	void valueChanged(QString);

public slots:
	/**
	 * @brief Slot function invoked when editing of the QLineEdit is finished.
	 *
	 * This function is called when editing of the QLineEdit sub-widget (m_lineEdit) is finished.
	 * It retrieves the text value from the QLineEdit, emits a signal indicating that the value has changed,
	 * and passes the updated value as a QString.
	 *
	 * @note This function assumes that the AbstractLineEdit contains a QLineEdit sub-widget (m_lineEdit).
	 *       If m_lineEdit is not initialized or nullptr, the behavior is undefined.
	 *
	 * @todo Implement validation logic to ensure the given value is valid before emitting the signal.
	 */
	void onEditingFinished();
};

#endif