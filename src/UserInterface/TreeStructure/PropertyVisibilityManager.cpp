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
* File      : PropertyVisibilityManager.cpp
* Author    : Paweł Gilewicz
* Date      : 27/02/2025
*/

#include "PropertyVisibilityManager.hpp"
#include "PropertiesModel.hpp"
#include "PropertiesWidget.hpp"
#include "spdlog/spdlog.h"

#include <QApplication>
#include <QTreeWidget>

//-----------------------------------------------------------------------------
PropertyVisibilityManager::PropertyVisibilityManager(QObject* parent)
	: QObject(parent) { }

//-----------------------------------------------------------------------------
void PropertyVisibilityManager::registerVisibilityRule(
	const QModelIndex& triggerIndex, const QList<QModelIndex>& affectedIndices,
	PropertiesModel* model) {

	for (const VisibilityRule& rule : _visibilityRules) {
		if (rule.triggerIndex == triggerIndex
			&& rule.affectedIndices == affectedIndices
			&& rule.parentModel == model) {
			// SPDLOG_WARN("Display rule already exists for widget: {}",
			// 	triggerIndex.row());
			return;
		}
	}

	_visibilityRules.append({ triggerIndex, affectedIndices, model });
}

//-----------------------------------------------------------------------------
void PropertyVisibilityManager::updateViewAttributes(
	const QModelIndex& changedIndex, const QVariant& newValue) {

	for (auto& rule : _visibilityRules) {

		if (rule.triggerIndex == changedIndex) {
			const bool checked = newValue.toBool();

			for (const auto& index : rule.affectedIndices) {
				if (QDomElement property
					= rule.parentModel->getProperty(index.row());
					!property.isNull()) {
					property.setAttribute("hidden", checked ? "no" : "yes");
				}
			}
		}
	}
	const auto propertiesWidget
		= qApp->property("PropertiesWidget").value<PropertiesWidget*>();
	propertiesWidget->updateView();
}