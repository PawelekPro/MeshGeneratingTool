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

#include "PropertiesWidget.h"

//--------------------------------------------------------------------------------------
PropertiesWidget::~PropertiesWidget() {
	QAbstractItemModel* model = this->model();
	if (model) {
		ModelFilter* proxy = qobject_cast<ModelFilter*>(model);
		;
		if (proxy) {
			for (int i = 0; i < proxy->rowCount(); ++i) {
				QModelIndex index = proxy->index(i, 1);
				QWidget* widget = indexWidget(index);
				delete widget;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
void PropertiesWidget::setModel(PropertiesModel* model) {
    qDeleteAll(_createdWidgets);
    _createdWidgets.clear();

    ModelFilter* proxy = new ModelFilter(this);
    proxy->setSourceModel(model);
    QTableView::setModel(proxy);

    if (proxy) {
        for (int i = 0; i < proxy->rowCount(); ++i) {
            this->setRowHeight(i, this->rowHeight);
            QModelIndex index = proxy->index(i, 1);
            QModelIndex indexSource = proxy->mapToSource(index);
            QWidget* widget = model->getWidget(indexSource);

            this->setIndexWidget(index, widget);
            _createdWidgets.append(widget);
        }
    }
}