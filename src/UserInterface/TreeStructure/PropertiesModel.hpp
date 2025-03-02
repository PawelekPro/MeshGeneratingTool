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

#ifndef PROPERTIESMODEL_HPP
#define PROPERTIESMODEL_HPP

#include <QSortFilterProxyModel>
#include <QAbstractTableModel>
#include <QString>
#include <QStringList>
#include <QTableView>
#include <Qt>
#include <QtXml/QDomElement>

#include "WidgetFactory.hpp"

/**
 * Custom model filter class that filters rows based on a custom criteria.
 *
 * This class inherits from QSortFilterProxyModel and overrides the filterAcceptsRow method
 * to provide custom filtering logic.
 */
class ModelFilter final : public QSortFilterProxyModel {
	Q_OBJECT

public:
	explicit ModelFilter(QObject *parent = nullptr)
		: QSortFilterProxyModel(parent) {
	}

	~ModelFilter() override = default;

protected:
	/**
	 * Determines if a row should be accepted by the filter.
	 *
	 * @param source_row The row index in the source model.
	 * @param source_parent The parent index in the source model.
	 *
	 * @returns True if the row should be accepted, false otherwise.
	 */
	[[nodiscard]] bool
	filterAcceptsRow(
		int source_row, const QModelIndex &source_parent) const override;
};

/**
 * A custom interface for displaying properties of a QDomElement in a
 * QtTableView based widget.
 */
class PropertiesModel final : public QAbstractTableModel {
	Q_OBJECT

public:
	enum Col {
		Label,
		Data
	};

	explicit PropertiesModel(
		const QDomElement &element, QWidget *parent = nullptr);

	~PropertiesModel() override;

	/**
	 * Returns the number of rows under the given parent index.
	 *
	 * @param parent The parent index. If not specified, the root index is used.
	 * @return The number of rows under the parent index.
	 */
	[[nodiscard]] int rowCount(const QModelIndex &parent) const
	Q_DECL_OVERRIDE;

	/**
	 * Returns the number of columns in the model under the given parent index.
	 *
	 * @param parent The parent index in the model (default is the root index).
	 * @return The number of columns in the model under the specified parent index.
	 */
	[[nodiscard]] int columnCount(const QModelIndex &parent) const
	Q_DECL_OVERRIDE;

	/**
	 * Returns the data for the specified role and index in the model.
	 *
	 * @param index The index of the item in the model.
	 * @param role The role for which the data is required (default is Qt::DisplayRole).
	 *
	 * @returns A QVariant containing the data for the specified role and index.
	 */
	[[nodiscard]] QVariant data(const QModelIndex &index, int role) const
	Q_DECL_OVERRIDE;

	/**
	 * Sets the data for the item at the specified index with the given value and role.
	 *
	 * @param index The model index of the item.
	 * @param value The new value to set for the item.
	 * @param role The role for which the data is being set.
	 *
	 * @returns True if the data was successfully set, false otherwise.
	 */
	bool setData(const QModelIndex &index, const QVariant &value, int role)
	Q_DECL_OVERRIDE;

	/**
	 * Returns the data for the header section specified by the given role, section, and orientation.
	 *
	 * @param section The index of the header section.
	 * @param orientation The orientation of the header (horizontal or vertical).
	 * @param role The role of the header data to be retrieved.
	 *
	 * @returns A QVariant containing the data for the specified header section.
	 */
	[[nodiscard]] QVariant headerData(
		int section, Qt::Orientation orientation,
		int role) const Q_DECL_OVERRIDE;

	/**
	 * Returns the item flags for the given index.
	 *
	 * This function is an override of the base class function in Qt.
	 *
	 * @param index The model index for which to retrieve the item flags.
	 * @return The item flags for the specified index.
	 */
	[[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const
	Q_DECL_OVERRIDE;

	/**
	 * Returns the QDomElement corresponding to the property with the given integer identifier.
	 *
	 * @param id The integer identifier of the property.
	 * @return The QDomElement representing the property.
	 */
	[[nodiscard]] QDomElement getProperty(int) const;

	/**
	 * Returns the QDomElement corresponding to the property with the given integer identifier.
	 *
	 * @param index
	 * @param row row of the changed property
	 * @param name name of the property to change
	 * @param value value to be assigned to the property
	 * @return The QDomElement representing the property.
	 */
	void setElementProperty(
		const QModelIndex &index, const QString &name,
		const QVariant &value);

	/**
	 * @brief creates and returns QWidget based on the name stored in QDomElement that is stored
	 * in QMap<int, QDomElement>. The element is accessed via aIndex.
	 *
	 * @param aIndex - index of QDomElement in _properties QMap that will be used to get name of
	 * widget to be created.
	 * @param aWidgetParent - parent that will be set to the newly created widget
	 */
	QWidget *getWidget(const QModelIndex &aIndex, QWidget *aWidgetParent);

private:
	QDomElement _element; // Pointer to a QDomElement object.

	QMap<int, QDomElement> _properties; // Map of properties

	QStringList _header; // A list of header strings.
};

#endif
