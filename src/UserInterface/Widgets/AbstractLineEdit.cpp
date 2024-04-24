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
#include "PropertiesModel.h"

// clang-format off
QString AbstractLineEdit::labelStyleSheet = QString(R"(
	padding-left: 1px;
	font-size: %1pt;
)").arg(AbstractLineEdit::fontSize);

QString AbstractLineEdit::lineEditStyleSheet = QString(R"(
	border: 0px;
	font-size: %1pt;
)").arg(AbstractLineEdit::fontSize);
// clang-format on

//----------------------------------------------------------------------------
AbstractLineEdit::AbstractLineEdit(QWidget* parent, QValidator* validator)
	: BaseWidget(parent)
	, m_lineEdit(new QLineEdit(this))
	, m_suffixLabel(new QLabel(this))
	, m_lastValidValue(0)
	, m_minVal(-INFINITY)
	, m_maxVal(INFINITY)
	, m_locale(QLocale()) {

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	m_lineEdit->setFixedHeight(this->lineHeight);
	m_suffixLabel->setFixedHeight(this->lineHeight);
	layout->addWidget(m_suffixLabel);

	if (validator != nullptr) {
		m_lineEdit->setValidator(validator);
	}
	layout->insertWidget(0, m_lineEdit);

	// Plain/StylePanel style
	m_suffixLabel->setFrameStyle(QFrame::StyledPanel);

	this->m_lineEdit->setStyleSheet(this->lineEditStyleSheet);
	this->m_suffixLabel->setStyleSheet(this->labelStyleSheet);

	QObject::connect(m_lineEdit, SIGNAL(editingFinished()),
		this, SLOT(onEditingFinished()));

	this->setLayout(layout);
}

//----------------------------------------------------------------------------
AbstractLineEdit::~AbstractLineEdit() {
	delete m_suffixLabel;
	delete m_lineEdit;
}

//----------------------------------------------------------------------------
void AbstractLineEdit::initialize(const QModelIndex& index) {
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

	// Conversion from QDomNamedNodeMap to QXmlAttributes
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

//----------------------------------------------------------------------------
void AbstractLineEdit::setValue(const std::string& value) {
	m_lineEdit->setText(QString::fromStdString(value));
}

//----------------------------------------------------------------------------
std::string AbstractLineEdit::getValue() {
	std::string text = m_lineEdit->text().toStdString();
	return text;
}

//----------------------------------------------------------------------------
void AbstractLineEdit::onEditingFinished() {
	const QString value = m_lineEdit->text();

	// ToDo: check if given value is valid
	emit this->valueChanged(value);
}