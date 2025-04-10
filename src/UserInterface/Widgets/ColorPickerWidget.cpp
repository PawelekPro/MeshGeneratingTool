/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
 * (https://github.com/PawelekPro/MeshGeneratingTool)
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

#include "ColorPickerWidget.hpp"

#include <QHBoxLayout>
#include <QModelIndex>
#include <QPushButton>

//----------------------------------------------------------------------------
ColorPickerWidget::ColorPickerWidget(QWidget* parent)
	: BaseWidget(parent)
	, _selectionButton(new QPushButton(this))
	, _index(QModelIndex())
	, _color(QColor()) {
	auto* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_selectionButton);

	connect(_selectionButton, &QPushButton::clicked, this,
		&ColorPickerWidget::onSelectColorClicked);

	const auto aButtonStylesheet = QString("border: none;");
	_selectionButton->setStyleSheet(aButtonStylesheet);

	this->setValue(std::make_tuple(100, 100, 100));
	this->setLayout(layout);
}

//----------------------------------------------------------------------------
ColorPickerWidget::~ColorPickerWidget() = default;

//----------------------------------------------------------------------------
void ColorPickerWidget::setIndex(const QModelIndex& index) {
	_index = index;

	if (const QVariant val = index.model()->data(index); val.isValid()) {
		const QString aColorStr = val.toString();
		QStringList aColorList = aColorStr.split(',');
		int r = aColorList[0].toInt();
		int g = aColorList[1].toInt();
		int b = aColorList[2].toInt();
		setValue(std::make_tuple(r, g, b));
	}
}

//----------------------------------------------------------------------------
void ColorPickerWidget::setValue(const std::tuple<int, int, int>& color) {
	const int r = std::get<0>(color);
	const int g = std::get<1>(color);
	const int b = std::get<2>(color);
	_color.setRgb(r, g, b);

	const auto aColor = QColor(r, g, b);
	const QString aStyleSheet
		= QString("background-color: %1").arg(aColor.name());
	this->setStyleSheet(aStyleSheet);

	const QString aColorStr = QString("%1, %2, %3").arg(r).arg(g).arg(b);

	if (_index.isValid()) {
		auto* model = const_cast<QAbstractItemModel*>(_index.model());
		model->setData(_index, QVariant(aColorStr));
	}
}

//----------------------------------------------------------------------------
void ColorPickerWidget::onSelectColorClicked() {
	QColorDialog dlg;

	if (_index.isValid()) {
		const QString aColorStr = _index.model()->data(_index).toString();
		QStringList aColorList = aColorStr.split(',');
		const int r = aColorList[0].toInt();
		const int g = aColorList[1].toInt();
		const int b = aColorList[2].toInt();
		const auto aColor = QColor(r, g, b);

		dlg.setCurrentColor(aColor);
	}

	if (dlg.exec() == QDialog::Accepted) {
		const QColor newColor = dlg.currentColor();
		setValue(
			std::make_tuple(newColor.red(), newColor.green(), newColor.blue()));
	}
}

//----------------------------------------------------------------------------
double* ColorPickerWidget::getColorAsDoubleArray() {
	_rgb[0] = _color.redF();
	_rgb[1] = _color.greenF();
	_rgb[2] = _color.blueF();

	return _rgb;
}
