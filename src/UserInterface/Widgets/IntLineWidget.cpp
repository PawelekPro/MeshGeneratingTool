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

#include "IntLineWidget.h"

//----------------------------------------------------------------------------
IntLineWidget::IntLineWidget(QWidget* parent)
	: AbstractLineEdit(parent, new QIntValidator()) {
	m_index = QModelIndex();
}

//----------------------------------------------------------------------------
void IntLineWidget::setIndex(const QModelIndex& index) {
	AbstractLineEdit::initialize(index);
	m_index = index;

	QVariant val = index.model()->data(index);
	if (val.isValid()) {
		this->setValue(std::to_string(val.toInt()));
	}

	if (QObject::receivers(SIGNAL(valueChanged(int))) > 0) {
		QObject::disconnect(
			this, SIGNAL(valueChanged(QString)),
			this, SLOT(onValueChanged(QString)));
	}
	QObject::connect(this, SIGNAL(valueChanged(QString)),
		this, SLOT(onValueChanged(QString)));
}

//----------------------------------------------------------------------------
void IntLineWidget::onValueChanged(QString value) {
	// qDebug() << value.toInt();

	QModelIndex mutableIndex = m_index; // Create a non-const copy
	if (mutableIndex.model()) { // Check if the model is valid
		QAbstractItemModel* mutableModel = const_cast<QAbstractItemModel*>(mutableIndex.model());
		mutableModel->setData(mutableIndex, value);
	} else {
		qWarning() << "Invalid model in QModelIndex.";
	}
}