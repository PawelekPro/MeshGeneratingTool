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

class ModelFilter : public QSortFilterProxyModel {
	Q_OBJECT

public:
	explicit ModelFilter(QObject* parent = nullptr)
		: QSortFilterProxyModel(parent) { }
	~ModelFilter() = default;

protected:
	bool filterAcceptsRow(int, const QModelIndex&) const override;
};

class PropertiesModel : public QAbstractTableModel {
	Q_OBJECT
public:
	PropertiesModel(QDomElement* element, QWidget* parent = nullptr);
	~PropertiesModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

	int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	bool setData(const QModelIndex& index, const QVariant& value, int role) Q_DECL_OVERRIDE;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

	Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

private:
	QDomElement* _element;
	QList<QDomElement> _properties;
	QStringList _header;
};

#endif