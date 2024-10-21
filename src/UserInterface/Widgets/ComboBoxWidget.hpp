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

#ifndef COMBOBOXWIDGET_HPP
#define COMBOBOXWIDGET_HPP

#include "AppDefaults.h"
#include "BaseWidget.hpp"

#include "document.h"
#include "filereadstream.h"

#include <QAbstractItemModel>
#include <QComboBox>
#include <QFile>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QStringListModel>
#include <QWidget>

#include <vtkLogger.h>

/**
 * @brief A custom widget for displaying and selecting options from a ComboBox.
 *
 * The ComboBoxWidget class extends the functionality of the BaseWidget to provide a specialized widget
 * for displaying and selecting options from a ComboBox. It initializes the model of the ComboBox based on
 * the data retrieved from a specified QModelIndex, expecting the model associated with the index to be of type PropertiesModel.
 *
 * This class is intended to be used for presenting selectable options to users in a graphical user interface.
 *
 * @note This class inherits from BaseWidget and adds functionality specific to ComboBox interaction.
 *
 * @see BaseWidget
 */
class ComboBoxWidget : public BaseWidget {
	Q_OBJECT
public:
	/**
	 * @brief Constructs a ComboBoxWidget object.
	 *
	 * @param parent The parent widget to which this widget belongs.
	 */
	ComboBoxWidget(QWidget* parent = nullptr);

	/**
	 * @brief Destroys the ComboBoxWidget object.
	 */
	~ComboBoxWidget();

	/**
	 * @brief Sets the QModelIndex associated with the ComboBoxWidget and initializes its model.
	 *
	 * This function sets the QModelIndex associated with the ComboBoxWidget and initializes its model
	 * based on the data retrieved from the specified QModelIndex. It expects the model associated with
	 * the index to be of type PropertiesModel.
	 *
	 * @param index The QModelIndex from which to initialize the widget and its model.
	 * @throws std::invalid_argument if the model assigned to the index is not of the 'PropertiesModel' type.
	 *
	 * The function retrieves the QDomElement associated with the given index from the PropertiesModel.
	 * It then extracts attributes such as 'model' from the QDomElement to determine the model name.
	 *
	 * A QStringListModel is created based on the model name, and the ComboBoxWidget is set to use this model.
	 *
	 * @note This function assumes that the model contains properties such as 'model' associated with the QModelIndex.
	 *       If the model or the properties are missing or malformed, the behavior is undefined.
	 */
	void setIndex(const QModelIndex& index) override;

private:
	/**
	 * @brief Creates a QStringListModel based on the specified model name.
	 *
	 * This function creates a QStringListModel based on the model name provided.
	 * It retrieves model data from a JSON document and constructs the QStringListModel
	 * using the data found in the document. If the model name is not found in the document,
	 * an error is logged, and an empty QStringListModel is returned.
	 *
	 * @param name The name of the model to create.
	 * @return A QStringListModel representing the data of the specified model name.
	 *
	 * The function retrieves the model data from a JSON document based on the provided model name.
	 * It constructs the QStringListModel by iterating over the model data, sorting it by IDs,
	 * and populating a QStringList with corresponding labels.
	 *
	 * @note This function assumes that the document member variable contains a valid JSON document
	 *       and that the model data is structured according to the expected format.
	 *       If the model data is missing or malformed, the behavior is undefined.
	 */
	QStringListModel* createQStringListModel(const QString&);

	QComboBox* _comboBox; /**< Pointer to the ComboBox widget. */
	QModelIndex _index; /**< The QModelIndex associated with the ComboBoxWidget. */

	// rapidjson document
	rapidjson::Document* document;
};

#endif