/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef ENTITYPICKWIDHET_H
#define ENTITYPICKWIDHET_H

#include "BaseWidget.h"
#include "PropertiesModel.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
#include <QString>
#include <QWidget>

class EntityPickWidget : public BaseWidget {
	Q_OBJECT
public:
	explicit EntityPickWidget(QWidget* parent = nullptr);
	~EntityPickWidget();

	void setIndex(const QModelIndex& index) override;
	void setSelected(bool selected);

private:
	void updateAppearance();


	QLabel* _selectionLabel;
	QPushButton* _selectionButton;
	QModelIndex _index;
	TreeWidgetEventHandler* _eventHandler;
	bool _selected;

	// button width in pixels
	static const int buttonWidth = 60;

protected:
	void mousePressEvent(QMouseEvent* event) override;

signals:
	void confirmed(const QString& selection);

private slots:
	void confirmSelection();
	void updateSelectedNames(const std::vector<std::string>& selectedNames, std::vector<int> selectedTags);
};

#endif