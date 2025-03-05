/*
* Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
	(https://github.com/PawelekPro/MeshGeneratingTool)
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

*=============================================================================
* File      : PropertiesWidget.cpp
* Author    : Paweł Gilewicz
* Date      : 05/03/2025
*/

#include "PropertiesWidget.hpp"
#include "TreeItem.hpp"

#include <QApplication>

#include "spdlog/spdlog.h"

#include <QHeaderView>

//-----------------------------------------------------------------------------
PropertiesWidget::PropertiesWidget(QWidget* parent)
	: QTableView(parent)
	, _proxyModel((nullptr))
	, _propertiesModel(nullptr) {
	qApp->setProperty("PropertiesWidget", QVariant::fromValue(this));
}

//-----------------------------------------------------------------------------
PropertiesWidget::~PropertiesWidget() {
	if (_proxyModel) {
		delete _proxyModel;
		_proxyModel = nullptr;
	}

	qDeleteAll(_widgetsMap);
	_widgetsMap.clear();
}

//-----------------------------------------------------------------------------
void PropertiesWidget::setModel(PropertiesModel* aModel) {
	for (const auto widget : _widgetsMap) {
		widget->deleteLater();
	}
	_widgetsMap.clear();

	_propertiesModel = aModel;

	if (_proxyModel) {
		delete _proxyModel;
	}

	_proxyModel = new ModelFilter(this);
	_proxyModel->setSourceModel(aModel);
	QTableView::setModel(_proxyModel);

	for (int i = 0; i < _proxyModel->rowCount(); ++i) {
		this->setRowHeight(i, PropertiesWidget::_rowHeight);
		QModelIndex index = _proxyModel->index(i, PropertiesModel::Col::Data);
		QModelIndex indexSource = _proxyModel->mapToSource(index);

		QWidget* widget = aModel->getWidget(indexSource, this);
		this->setIndexWidget(index, widget);

		if (widget && !_widgetsMap.contains(widget)) {
			_widgetsMap.append(widget);
		}
	}
}

//-----------------------------------------------------------------------------
void PropertiesWidget::updateView() {
	spdlog::debug("Requested update view event in PropertiesWidget class");
	QTableView::setModel(nullptr);
	setModel(_propertiesModel);

	QTableView::resizeColumnToContents(0);
}
