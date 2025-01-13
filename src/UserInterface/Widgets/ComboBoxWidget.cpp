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

#include "ComboBoxWidget.hpp"
#include "PropertiesModel.hpp"
#include "DocUtils.hpp"

//----------------------------------------------------------------------------
ComboBoxWidget::ComboBoxWidget(QWidget* parent)
	: BaseWidget(parent)
	, _comboBox(new QComboBox(this)) {

	_index = QModelIndex();

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_comboBox);

	this->setLayout(layout);
}

//----------------------------------------------------------------------------
ComboBoxWidget::~ComboBoxWidget() {
	_comboBox->deleteLater();
}

//----------------------------------------------------------------------------
void ComboBoxWidget::setIndex(const QModelIndex& index) {
	_index = index;

	const QAbstractItemModel* constModel = index.model();
	QAbstractItemModel* model = const_cast<QAbstractItemModel*>(constModel);
	PropertiesModel* propsModel = dynamic_cast<PropertiesModel*>(model);

	if (!propsModel) {
		throw std::invalid_argument(
			"The model assigned to the index must be of the 'PropertiesModel' type.");
	}

	QDomElement element = propsModel->getProperty(index.row());
	QString modelLabel = Properties::getPropertyAttribute(element, "model");
	QStringListModel* listModel = this->createQStringListModel(modelLabel);
	_comboBox->setModel(listModel);
}

//----------------------------------------------------------------------------
QStringListModel* ComboBoxWidget::createQStringListModel(const QString& name) {
	QStringList comboBoxList = DefaultsParser::getComboBoxList(name);
	QStringListModel* listModel = new QStringListModel(comboBoxList);
	return listModel;

}
