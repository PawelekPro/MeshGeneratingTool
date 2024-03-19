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
#include <Qt>

class ModelFilter : public QSortFilterProxyModel {
protected:
	ModelFilter() = default;
	~ModelFilter() = default;

	bool filterAcceptsRow(int, const QModelIndex&) const override;
};

class PropertiesModel : public QAbstractTableModel {
public:
	PropertiesModel(QDomElement* element, QObject* parent = nullptr);
	~PropertiesModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
	QDomElement* _element;
	QList<QDomElement> _properties;
	QStringList _header;
};

#endif