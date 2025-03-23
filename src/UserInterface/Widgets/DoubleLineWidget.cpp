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

#include "DoubleLineWidget.hpp"

//----------------------------------------------------------------------------
DoubleLineWidget::DoubleLineWidget(QWidget* parent)
	: AbstractLineEdit(parent, new QDoubleValidator()) {
	_index = QModelIndex();
}

//----------------------------------------------------------------------------
void DoubleLineWidget::setIndex(const QModelIndex& index) {
	AbstractLineEdit::initialize(index);
	_index = index;

	if (QVariant val = index.model()->data(index); val.isValid()) {
		this->setValue(val.toString().toStdString());
	}

	if (QObject::receivers(SIGNAL(valueChanged(QString))) > 0) {
		QObject::disconnect(this, SIGNAL(valueChanged(QString)), this,
			SLOT(onValueChanged(QString)));
	}
	QObject::connect(this, SIGNAL(valueChanged(QString)), this,
		SLOT(onValueChanged(QString)));
}

//----------------------------------------------------------------------------
void DoubleLineWidget::onValueChanged(QString value) {
	// qDebug() << value.toDouble();

	QModelIndex mutableIndex = _index; // Create a non-const copy
	if (mutableIndex.model()) { // Check if the model is valid
		QAbstractItemModel* mutableModel
			= const_cast<QAbstractItemModel*>(mutableIndex.model());
		mutableModel->setData(mutableIndex, value);

	} else {
		qWarning() << "Invalid model in QModelIndex.";
	}
}