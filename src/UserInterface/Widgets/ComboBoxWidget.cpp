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

#include "ComboBoxWidget.hpp"
#include "DocUtils.hpp"
#include "PropertiesModel.hpp"

#include <QHBoxLayout>

#include <spdlog/spdlog.h>

//----------------------------------------------------------------------------
ComboBoxWidget::ComboBoxWidget(QWidget* parent)
	: BaseWidget(parent)
	, _comboBox(new QComboBox(this)) {

	_index = QModelIndex();

	const auto layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_comboBox);

	this->setLayout(layout);
	connect(_comboBox, &QComboBox::currentIndexChanged, this,
		&ComboBoxWidget::onComboBoxIndexChanged);
}

//----------------------------------------------------------------------------
ComboBoxWidget::~ComboBoxWidget() = default;

//----------------------------------------------------------------------------
void ComboBoxWidget::setIndex(const QModelIndex& index) {
	_index = index;

	const QAbstractItemModel* constModel = index.model();
	auto* model = const_cast<QAbstractItemModel*>(constModel);
	auto* propsModel = dynamic_cast<PropertiesModel*>(model);

	if (!propsModel) {
		throw std::invalid_argument("The model assigned to the index must be "
									"of the 'PropertiesModel' type.");
	}

	const QDomElement element = propsModel->getProperty(index.row());
	const QString modelLabel
		= Properties::getPropertyAttribute(element, "model");

	QStringListModel* listModel = this->createQStringListModel(modelLabel);
	const bool old = _comboBox->blockSignals(true);

	if (_comboBox->model()) {
		_comboBox->model()->deleteLater();
	}
	_comboBox->setModel(listModel);

	const QString modelIndex = Properties::getPropertyValue(element);
	_comboBox->setCurrentIndex(modelIndex.toInt());
	_comboBox->blockSignals(old);
}

void ComboBoxWidget::onComboBoxIndexChanged(const int aIndex) const {
	if (!_index.isValid())
		return;

	const QString selectedValue = _comboBox->itemText(aIndex);

	auto* model = const_cast<QAbstractItemModel*>(_index.model());
	if (!model) {
		SPDLOG_ERROR("Model is null!");
		return;
	}

	if (!model->setData(_index, aIndex, Qt::EditRole)) {
		SPDLOG_ERROR("Failed to set data on the model!");
	} else {
		spdlog::debug(
			"Set data {} on the model", selectedValue.toStdString().data());
	}
}

//----------------------------------------------------------------------------
QStringListModel* ComboBoxWidget::createQStringListModel(
	const QString& name) const {
	const QStringList comboBoxList = DefaultsParser::getComboBoxList(name);
	return new QStringListModel(comboBoxList, _comboBox);
}
