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
	: _selectionLabel(new QLabel(this))
	, _selectionButton(new QPushButton("Select", this))
	, _selected(false) {

	_index = QModelIndex();

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_selectionLabel);
	layout->addWidget(_selectionButton);

	_selectionButton->setFixedWidth(this->buttonWidth);
	_selectionButton->hide();

	connect(_selectionButton, &QPushButton::clicked, this, &EntityPickWidget::confirmSelection);
	this->setLayout(layout);
}

//----------------------------------------------------------------------------
EntityPickWidget::~EntityPickWidget() {
	_selectionButton->deleteLater();
	_selectionLabel->deleteLater();
}

//----------------------------------------------------------------------------
void EntityPickWidget::setIndex(const QModelIndex& index) {
	_index = index;

	PropertiesModel* model = dynamic_cast<PropertiesModel*>(const_cast<QAbstractItemModel*>(this->_index.model()));
	this->_eventHandler = model->eventHandler;

	connect(this->_eventHandler, &TreeWidgetEventHandler::selectedEntitiesNamesFetched, this, &EntityPickWidget::updateSelectedNames);
}

//----------------------------------------------------------------------------
void EntityPickWidget::updateAppearance() {
	if (_selected) {
		_selectionButton->show();
	} else {
		_selectionLabel->setText("");
		_selectionButton->hide();
	}
}

//----------------------------------------------------------------------------
void EntityPickWidget::confirmSelection() {
	if (_selected) {
		this->updateAppearance();
	}
	_selectionButton->hide();
	emit _eventHandler->entitySelectionConfirmed();
}

//----------------------------------------------------------------------------
void EntityPickWidget::mousePressEvent(QMouseEvent* event) {
	if (!_selected) {
		this->setSelected(true);
	}
	_selectionButton->show();
}

//----------------------------------------------------------------------------
void EntityPickWidget::setSelected(bool selected) {
	_selected = selected;
}

void EntityPickWidget::updateSelectedNames(const std::vector<std::string>& selectedNames){
	QString qMergedNames = "";
	if(!selectedNames.empty()){
		std::string mergedNames = std::accumulate(
					selectedNames.begin() + 1, 
					selectedNames.end(), 
					selectedNames[0], 
					[](const std::string& a, const std::string& b) {
						return a + "; " + b;
					}
				);
		qMergedNames = QString::fromStdString(mergedNames);
	}
	QModelIndex mutableIndex = _index;
	if (mutableIndex.model()) {
		QAbstractItemModel* mutableModel
			= const_cast<QAbstractItemModel*>(mutableIndex.model());
		mutableModel->setData(mutableIndex, qMergedNames);
	} else {
		qWarning() << "Invalid model in QModelIndex.";
	}
}


