/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#include "EntityPickWidget.hpp"
#include "DocUtils.hpp"
#include "ModelActionsHandler.hpp"
#include "PropertiesModel.hpp"
#include "RenderSignalSender.hpp"
#include "TreeStructure.hpp"
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

	connect(_selectionButton, &QPushButton::clicked, this,
		&EntityPickWidget::confirmSelection);
	this->setLayout(layout);
}

//----------------------------------------------------------------------------
EntityPickWidget::~EntityPickWidget() = default;

//----------------------------------------------------------------------------
void EntityPickWidget::setIndex(const QModelIndex& index) {
	_index = index;
	QAbstractItemModel* sourceModel
		= const_cast<QAbstractItemModel*>(this->_index.model());
	_propModel = qobject_cast<PropertiesModel*>(sourceModel);
	if (_propModel) {
		TreeStructure* treeStrucutre
			= qobject_cast<TreeStructure*>(_propModel->parent());
		if (treeStrucutre) {
			_signalSender = treeStrucutre->modelHandler()
								->_renderSignalSender->geometrySignals;
		} else {
			qDebug("Parent of widgets PropertiesModel should be TreeStructure");
		}
	} else {
		qDebug("Casting to Properties model failed - widget can only use "
			   "PropertiesModel class");
	}
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
	std::vector<int> selectedShapes = _signalSender->getSelectedShapes();
	QString selectedShapesString = DocUtils::intsToString(selectedShapes);
	_propModel->setData(
		_index, QVariant::fromValue(selectedShapesString), Qt::EditRole);
	if (_selected) {
		this->updateAppearance();
	}
	_selectionButton->hide();
}

//----------------------------------------------------------------------------
void EntityPickWidget::mousePressEvent(QMouseEvent* event) {
	if (!_selected) {
		this->setSelected(true);
	}
	_selectionButton->show();
}

//----------------------------------------------------------------------------
void EntityPickWidget::setSelected(bool selected) { _selected = selected; }