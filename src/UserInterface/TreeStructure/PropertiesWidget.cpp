/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
 * (https://github.com/PawelekPro/MeshGeneratingTool)
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

#include "PropertiesWidget.hpp"
#include "TreeItem.hpp"

#include <QApplication>

//--------------------------------------------------------------------------------------
PropertiesWidget::PropertiesWidget(QWidget* parent)
	: QTableView(parent) {
	qApp->setProperty("PropertiesWidget", QVariant::fromValue(this));
	std::cout << "INITIAL PROPERTIES WIDGET" << std::endl;
}

//--------------------------------------------------------------------------------------
PropertiesWidget::~PropertiesWidget() {
	if (QAbstractItemModel* model = this->model()) {
		if (const auto proxy = qobject_cast<ModelFilter*>(model)) {
			for (int i = 0; i < proxy->rowCount(); ++i) {
				QModelIndex index = proxy->index(i, 1);
				const QWidget* widget = indexWidget(index);
				delete widget;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
void PropertiesWidget::setModel(PropertiesModel* aModel) {
	qDeleteAll(_createdWidgets);
	_createdWidgets.clear();

	if (QAbstractItemModel* model = this->model()) {
		std::cout << "CURRENT MODEL: " << model << std::endl;
		model->deleteLater();
	}

	const auto proxy = new ModelFilter(this);
	proxy->setSourceModel(aModel);
	aModel->setProxyFilter(proxy);

	QTableView::setModel(proxy);

	for (int i = 0; i < proxy->rowCount(); ++i) {
		this->setRowHeight(i, PropertiesWidget::_rowHeight);
		QModelIndex index = proxy->index(i, PropertiesModel::Col::Data);
		QModelIndex indexSource = proxy->mapToSource(index);

		QWidget* widget = aModel->getWidget(indexSource, this);
		this->setIndexWidget(index, widget);
		_createdWidgets.append(widget);
	}
}

//--------------------------------------------------------------------------------------
void PropertiesWidget::onModelDataChanged(PropertiesModel* aModel) {
	std::cout << "Data change registered " << aModel << std::endl;

	const auto proxy = new ModelFilter(this);
	proxy->setSourceModel(aModel);
	aModel->setProxyFilter(proxy);

	QTableView::setModel(proxy);

	for (int i = 0; i < proxy->rowCount(); ++i) {
		this->setRowHeight(i, PropertiesWidget::_rowHeight);
		QModelIndex index = proxy->index(i, PropertiesModel::Col::Data);
		QModelIndex indexSource = proxy->mapToSource(index);

		QWidget* widget = aModel->getWidget(indexSource, this);
		this->setIndexWidget(index, widget);
	}
}
