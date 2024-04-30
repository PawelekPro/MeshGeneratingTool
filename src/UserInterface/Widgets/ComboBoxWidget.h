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

#ifndef COMBOBOXWIDGET_H
#define COMBOBOXWIDGET_H

#include "AppDefaults.h"
#include "BaseWidget.h"

#include "document.h"
#include "filereadstream.h"

#include <QAbstractItemModel>
#include <QComboBox>
#include <QFile>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QStringListModel>
#include <QWidget>

#include <vtkLogger.h>

class ComboBoxWidget : public BaseWidget {
	Q_OBJECT
public:
	ComboBoxWidget(QWidget* parent = nullptr);
	~ComboBoxWidget();

	void setIndex(const QModelIndex& index) override;

private:
	QStringListModel* createQStringListModel(const QString&);

	QComboBox* m_comboBox;
	QModelIndex m_index;

	// rapidjson document
	rapidjson::Document* document;
};

#endif