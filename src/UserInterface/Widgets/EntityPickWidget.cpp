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

#include "EntityPickWidget.h"

//----------------------------------------------------------------------------
EntityPickWidget::EntityPickWidget(QWidget* parent)
	: m_selectionLabel(new QLabel(this))
	, m_selectionButton(new QPushButton("Select", this))
	, m_selected(false) {

	m_index = QModelIndex();

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_selectionLabel);
	layout->addWidget(m_selectionButton);

	m_selectionButton->setFixedWidth(this->buttonWidth);
	m_selectionButton->hide();

	connect(m_selectionButton, &QPushButton::clicked, this, &EntityPickWidget::confirmSelection);
	this->setLayout(layout);
}

//----------------------------------------------------------------------------
EntityPickWidget::~EntityPickWidget() {
	m_selectionButton->deleteLater();
	m_selectionLabel->deleteLater();
}

//----------------------------------------------------------------------------
void EntityPickWidget::setIndex(const QModelIndex& index) {
	m_index = index;
	// ToDo: To be developed...
}

//----------------------------------------------------------------------------
void EntityPickWidget::updateAppearance() {
	if (m_selected) {
		m_selectionLabel->setText("Selected");
		m_selectionButton->show();
	} else {
		m_selectionLabel->setText("");
		m_selectionButton->hide();
	}
}

//----------------------------------------------------------------------------
void EntityPickWidget::confirmSelection() {
	if (m_selected) {
		this->updateAppearance();
	}
	m_selectionButton->hide();
}

//----------------------------------------------------------------------------
void EntityPickWidget::mousePressEvent(QMouseEvent* event) {
	// ToDo: Accept or reject entity selection
	if (!m_selected) {
		this->setSelected(true);
	}
	m_selectionButton->show();
}

//----------------------------------------------------------------------------
void EntityPickWidget::setSelected(bool selected) {
	m_selected = selected;
}