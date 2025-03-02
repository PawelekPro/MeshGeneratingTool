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
* File      : CheckBoxWidget.cpp
* Author    : Paweł Gilewicz
* Date      : 02/03/2025
*/

#include "CheckBoxWidget.hpp"

#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
CheckBoxWidget::CheckBoxWidget(QWidget* parent)
	: BaseWidget(parent)
	, _checkBox(new QCheckBox(this))
	, _layout(new QVBoxLayout(this)) {

	_layout->addWidget(_checkBox);
	_layout->setContentsMargins(0, 0, 0, 0);
	_checkBox->setFixedHeight(_indicatorHeight);
	setLayout(_layout);

	connect(_checkBox, &QCheckBox::checkStateChanged, this,
		&CheckBoxWidget::onCheckStateChanged);
}

//----------------------------------------------------------------------------
CheckBoxWidget::~CheckBoxWidget() {
	delete _checkBox;
	delete _layout;
}

//----------------------------------------------------------------------------
void CheckBoxWidget::setIndex(const QModelIndex& index) {
	_index = index;

	if (const QVariant val = index.model()->data(index); val.isValid()) {
		const bool old = _checkBox->blockSignals(true);
		_checkBox->setChecked(val.toInt() == static_cast<int>(Qt::Checked));
		_checkBox->blockSignals(old);
	}
}

//----------------------------------------------------------------------------
void CheckBoxWidget::onCheckStateChanged() const {
	if (_index.isValid() && _index.model()) {
		auto* mutableModel = const_cast<QAbstractItemModel*>(_index.model());
		const QString newValue = QString::number(_checkBox->checkState());
		mutableModel->setData(_index, newValue);

		spdlog::debug("Setting value {} for widget at row {}",
			newValue.toStdString(), _index.row());
	} else {
		SPDLOG_WARN("Invalid model in QModelIndex.");
	}
}