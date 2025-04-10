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
* File      : ModelFilter.cpp
* Author    : Paweł Gilewicz
* Date      : 09/03/2025
*/

#include "ModelFilter.hpp"

#include "PropertiesModel.hpp"

#include <QDomElement>

//-----------------------------------------------------------------------------
ModelFilter::ModelFilter(QObject* parent)
	: QSortFilterProxyModel(parent) { }

//-----------------------------------------------------------------------------
bool ModelFilter::filterAcceptsRow(
	const int source_row, const QModelIndex& sourceParent) const {
	Q_UNUSED(sourceParent);

	QAbstractItemModel* model = this->sourceModel();

	const auto* propsModel = dynamic_cast<PropertiesModel*>(model);

	const QDomElement propsNode = propsModel->getProperty(source_row);
	const QString hidden
		= propsNode.attributes().namedItem("hidden").toAttr().value();

	return hidden == "no" || hidden.isEmpty();
}