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
* File      : TextLineWidget.cpp
* Author    : Paweł Gilewicz
* Date      : 27/02/2025
*/

#include "TextLineWidget.hpp"

#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
TextLineWidget::TextLineWidget(QWidget* parent)
	: BaseWidget(parent)
	, _lineEdit(new QLineEdit(this))
	, _layout(new QVBoxLayout(this)) {

	_layout->addWidget(_lineEdit);
	_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(_layout);

	connect(_lineEdit, &QLineEdit::editingFinished, this,
		&TextLineWidget::onEditingFinished);
}

//----------------------------------------------------------------------------
TextLineWidget::~TextLineWidget() {
	delete _lineEdit;
	delete _layout;
}

//----------------------------------------------------------------------------
void TextLineWidget::setIndex(const QModelIndex& index) {
	_index = index;

	if (const QVariant val = index.model()->data(index); val.isValid()) {
		const bool old = _lineEdit->blockSignals(true);
		_lineEdit->setText(val.toString());
		_lineEdit->blockSignals(old);
	}
}

//----------------------------------------------------------------------------
void TextLineWidget::onEditingFinished() const {
	if (_index.isValid() && _index.model()) {
		auto* mutableModel = const_cast<QAbstractItemModel*>(_index.model());
		const QString newValue = _lineEdit->text();
		mutableModel->setData(_index, newValue);
		spdlog::debug("Setting value {} for widget at row {}",
			newValue.toStdString(), _index.row());
	} else {
		SPDLOG_WARN("Invalid model in QModelIndex.");
	}
}