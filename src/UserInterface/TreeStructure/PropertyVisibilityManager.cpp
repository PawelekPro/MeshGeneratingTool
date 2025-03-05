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

#include <QTreeWidget>

//-----------------------------------------------------------------------------
PropertyVisibilityManager::PropertyVisibilityManager(QObject* parent)
	: QObject(parent) { }

//-----------------------------------------------------------------------------
void PropertyVisibilityManager::registerVisibilityRule(
	const QModelIndex& triggerIndex, const QVariant& expectedValue,
	const QList<QModelIndex>& affectedIndices) {

	for (const auto& rule : _visibilityRules) {
		if (rule.triggerIndex == triggerIndex
			&& rule.expectedValue == expectedValue
			&& rule.affectedIndices == affectedIndices) {
			return;
		}
	}

	_visibilityRules.append({ triggerIndex, expectedValue, affectedIndices });
}

//-----------------------------------------------------------------------------
void PropertyVisibilityManager::handleStateChange(
	const QModelIndex& changedIndex, const QVariant& newValue,
	PropertiesModel* model) {

	for (auto& rule : _visibilityRules) {
		std::cout << rule.expectedValue.toString().data() << std::endl;

		if (rule.triggerIndex == changedIndex) {
			const bool shouldHide
				= (rule.expectedValue.typeId() == QVariant::Bool)
				? (newValue.toBool() == rule.expectedValue.toBool())
				: (newValue.toString() == rule.expectedValue.toString());
			std::cout << shouldHide << std::endl;
			for (const auto& index : rule.affectedIndices) {
				if (QDomElement property = model->getProperty(index.row());
					!property.isNull()) {
					property.setAttribute("hidden", shouldHide ? "yes" : "no");

					// if (rule.newLabel.contains(newValue.toString())) {
					// 	property.setAttribute(
					// 		"label", rule.newLabel[newValue.toString()]);
					// }
				}
			}
		}
	}
	emit model->modelDataChanged(model);
}