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
#ifndef MODELFILTER_H
#define MODELFILTER_H

#include <QModelIndex>
#include <QSortFilterProxyModel>

/**
 * Custom model filter class that filters rows based on a custom criteria.
 *
 * This class inherits from QSortFilterProxyModel and overrides the
 * filterAcceptsRow method to provide custom filtering logic.
 */
class ModelFilter final : public QSortFilterProxyModel {
	Q_OBJECT

public:
	explicit ModelFilter(QObject* parent = nullptr);
	~ModelFilter() override = default;

protected:
	/**
	 * Determines if a row should be accepted by the filter.
	 *
	 * @param source_row The row index in the source model.
	 * @param source_parent The parent index in the source model.
	 *
	 * @returns True if the row should be accepted, false otherwise.
	 */
	[[nodiscard]] bool filterAcceptsRow(
		int source_row, const QModelIndex& source_parent) const override;
};

#endif
