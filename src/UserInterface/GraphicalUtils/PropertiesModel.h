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

#ifndef PROPERTIESMODEL_H
#define PROPERTIESMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QDomElement>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QString>
#include <QStringList>
#include <QTableView>
#include <Qt>

/**
 * Custom model filter class that filters rows based on a custom criteria.
 *
 * This class inherits from QSortFilterProxyModel and overrides the filterAcceptsRow method
 * to provide custom filtering logic.
 */
class ModelFilter : public QSortFilterProxyModel {
	Q_OBJECT

public:
	explicit ModelFilter(QObject* parent = nullptr)
		: QSortFilterProxyModel(parent) { }
	~ModelFilter() = default;

protected:
	/**
	 * Determines if a row should be accepted by the filter.
	 *
	 * @param source_row The row index in the source model.
	 * @param source_parent The parent index in the source model.
	 *
	 * @returns True if the row should be accepted, false otherwise.
	 */
	bool filterAcceptsRow(int, const QModelIndex&) const override;
};

/**
 * A custom interface for displaying properties of a QDomElement in a QtTableView based widget.
 */
class PropertiesModel : public QAbstractTableModel {
	Q_OBJECT
public:
	PropertiesModel(QDomElement* element, QWidget* parent = nullptr);
	~PropertiesModel();

	/**
	 * Returns the number of rows under the given parent index.
	 *
	 * @param parent The parent index. If not specified, the root index is used.
	 * @return The number of rows under the parent index.
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

	/**
	 * Returns the number of columns in the model under the given parent index.
	 *
	 * @param parent The parent index in the model (default is the root index).
	 * @return The number of columns in the model under the specified parent index.
	 */
	int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

	/**
	 * Returns the data for the specified role and index in the model.
	 *
	 * @param index The index of the item in the model.
	 * @param role The role for which the data is required (default is Qt::DisplayRole).
	 *
	 * @returns A QVariant containing the data for the specified role and index.
	 */
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	/**
	 * Sets the data for the item at the specified index with the given value and role.
	 *
	 * @param index The model index of the item.
	 * @param value The new value to set for the item.
	 * @param role The role for which the data is being set.
	 *
	 * @returns True if the data was successfully set, false otherwise.
	 */
	bool setData(const QModelIndex& index, const QVariant& value, int role) Q_DECL_OVERRIDE;

	/**
	 * Returns the data for the header section specified by the given role, section, and orientation.
	 *
	 * @param section The index of the header section.
	 * @param orientation The orientation of the header (horizontal or vertical).
	 * @param role The role of the header data to be retrieved.
	 *
	 * @returns A QVariant containing the data for the specified header section.
	 */
	QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

	/**
	 * Returns the item flags for the given index.
	 *
	 * This function is an override of the base class function in Qt.
	 *
	 * @param index The model index for which to retrieve the item flags.
	 * @return The item flags for the specified index.
	 */
	Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

private:
	/**
	 * Pointer to a QDomElement object.
	 */
	QDomElement* _element;

	/**
	 * A list of QDomElement objects representing properties.
	 */
	QList<QDomElement> _properties;

	/**
	 * A list of header strings.
	 */
	QStringList _header;
};

#endif