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
* File      : PropertyVisibilityManager.hpp
* Author    : Paweł Gilewicz
* Date      : 27/02/2025
*/
#ifndef PROPERTYVISIBILITYMANAGER_H
#define PROPERTYVISIBILITYMANAGER_H

#include <QAbstractItemView>
#include <QObject>
#include <QVariant>

class PropertiesModel;

struct VisibilityRule {
	QModelIndex triggerIndex;
	QVariant expectedValue;
	QList<QModelIndex> affectedIndices;
};

class PropertyVisibilityManager final : public QObject {
	Q_OBJECT

public:
	explicit PropertyVisibilityManager(QObject* parent = nullptr);

	void registerVisibilityRule(const QModelIndex& triggerIndex,
		const QVariant& expectedValue,
		const QList<QModelIndex>& affectedIndices);

	void handleStateChange(const QModelIndex& changedIndex,
		const QVariant& newValue, PropertiesModel* model);

private:
	QList<VisibilityRule> _visibilityRules {};
};

#endif // PROPERTYVISIBILITYMANAGER_H
