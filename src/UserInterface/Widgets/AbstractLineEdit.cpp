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
#include "AbstractLineEdit.h"

//----------------------------------------------------------------------------
AbstractLineEdit::AbstractLineEdit(QWidget* parent, QValidator* validator, QString castTo)
	: QWidget(parent)
	, m_lineEdit(new QLineEdit(this))
	, m_suffixLabel(new QLabel(this))
	, m_castTo(castTo)
	, m_lastValidValue(0)
	, m_minVal(-INFINITY)
	, m_maxVal(INFINITY)
	, m_locale(QLocale()) {

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	m_lineEdit->setFixedHeight(26);
	m_suffixLabel->setFixedHeight(26);
	layout->addWidget(m_suffixLabel);

	if (validator != nullptr) {
		m_lineEdit->setValidator(validator);
	}
	layout->insertWidget(0, m_lineEdit);
	this->setLayout(layout);
}

//----------------------------------------------------------------------------
void AbstractLineEdit::setIndex(QModelIndex index) {
	this->m_index = index;

	const QAbstractItemModel* constModel = index.model();
	QAbstractItemModel* model = const_cast<QAbstractItemModel*>(constModel);
	PropertiesModel* propsModel = dynamic_cast<PropertiesModel*>(model);

	if (!propsModel) {
		throw std::invalid_argument(
			"The model assigned to the index must be of the 'PropertiesModel' type.");
	}

	QDomElement element = propsModel->getProperty(index.row());
	QDomNamedNodeMap attrs = element.attributes();

	// Conversio from QDomNamedNodeMap to QXmlAttributes
	QXmlStreamAttributes xmlAttrs;
	for (int i = 0; i < attrs.count(); ++i) {
		QDomAttr attr = attrs.item(i).toAttr();
		xmlAttrs.append(attr.name(), attr.value());
	}

	QString suffix = xmlAttrs.value("unit").toString();
	double minVal = xmlAttrs.value("minVal").toString().toDouble();
	double maxVal = xmlAttrs.value("maxVal").toString().toDouble();

	this->m_suffixLabel->setText(suffix);
	this->m_minVal = minVal;
	this->m_maxVal = maxVal;
}