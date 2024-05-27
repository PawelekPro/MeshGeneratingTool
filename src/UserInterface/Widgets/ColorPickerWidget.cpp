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

#include "ColorPickerWidget.h"

//----------------------------------------------------------------------------
ColorPickerWidget::ColorPickerWidget(QWidget* parent)
	: BaseWidget(parent)
	, _selectionButton(new QPushButton(this))
	, _index(QModelIndex()) {

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_selectionButton);

	connect(_selectionButton, &QPushButton::clicked, this, &ColorPickerWidget::onSelectColorClicked);
	this->setValue(std::make_tuple(100, 100, 100));
	this->setLayout(layout);
}

//----------------------------------------------------------------------------
ColorPickerWidget::~ColorPickerWidget() {
	_selectionButton->deleteLater();
}

//----------------------------------------------------------------------------
void ColorPickerWidget::setIndex(const QModelIndex& index) {
	_index = index;

	QVariant val = index.model()->data(index);
	if (val.isValid()) {
		QString aColorStr = val.toString();
		QStringList aColorList = aColorStr.split(',');
		int r = aColorList[0].toInt();
		int g = aColorList[1].toInt();
		int b = aColorList[2].toInt();
		setValue(std::make_tuple(r, g, b));
	}
}

//----------------------------------------------------------------------------
void ColorPickerWidget::setValue(const std::tuple<int, int, int>& color) {
	int r = std::get<0>(color);
	int g = std::get<1>(color);
	int b = std::get<2>(color);

	QColor aColor = QColor(r, g, b);
	QString aStyleSheet = QString("background-color: %1").arg(aColor.name());
	this->setStyleSheet(aStyleSheet);

	QString aColorStr = QString("%1, %2, %3").arg(r).arg(g).arg(b);

	if (_index.isValid()) {
		QAbstractItemModel* model = const_cast<QAbstractItemModel*>(_index.model());
		model->setData(_index, QVariant(aColorStr));
	}
}

//----------------------------------------------------------------------------
void ColorPickerWidget::onSelectColorClicked() {
	QColorDialog dlg;

	if (_index.isValid()) {
		QString aColorStr = _index.model()->data(_index).toString();
		QStringList aColorList = aColorStr.split(',');
		int r = aColorList[0].toInt();
		int g = aColorList[1].toInt();
		int b = aColorList[2].toInt();
		QColor aColor = QColor(r, g, b);

		dlg.setCurrentColor(aColor);
	}

	if (dlg.exec() == QDialog::Accepted) {
		QColor newColor = dlg.currentColor();
		setValue(std::make_tuple(newColor.red(), newColor.green(), newColor.blue()));
	}
}