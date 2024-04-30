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

#include "ComboBoxWidget.h"
#include "PropertiesModel.h"

//----------------------------------------------------------------------------
ComboBoxWidget::ComboBoxWidget(QWidget* parent)
	: BaseWidget(parent)
	, document(new rapidjson::Document())
	, m_comboBox(new QComboBox(this)) {

	m_index = QModelIndex();

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_comboBox);

	this->document = AppDefaults::getInstance().getDocument();

	this->setLayout(layout);
}

//----------------------------------------------------------------------------
ComboBoxWidget::~ComboBoxWidget() {
	delete m_comboBox;
	// 'document' member is being deleted in AppDefaults class
}

//----------------------------------------------------------------------------
void ComboBoxWidget::setIndex(const QModelIndex& index) {
	m_index = index;

	const QAbstractItemModel* constModel = index.model();
	QAbstractItemModel* model = const_cast<QAbstractItemModel*>(constModel);
	PropertiesModel* propsModel = dynamic_cast<PropertiesModel*>(model);

	if (!propsModel) {
		throw std::invalid_argument(
			"The model assigned to the index must be of the 'PropertiesModel' type.");
	}

	QDomElement element = propsModel->getProperty(index.row());
	QDomNamedNodeMap attrs = element.attributes();
	QString modelName = attrs.namedItem("model").toAttr().value();

	QStringListModel* listModel = this->createQStringListModel(modelName);
	m_comboBox->setModel(listModel);
}

//----------------------------------------------------------------------------
QStringListModel* ComboBoxWidget::createQStringListModel(const QString& name) {

	if (!this->document->HasMember(name.toStdString().c_str())
		|| !(*this->document)[name.toStdString().c_str()].IsObject()) {

		vtkLogF(ERROR, ("No such model: " + name.toStdString()).c_str());
		return new QStringListModel();
	}

	const rapidjson::Value& model = (*this->document)[name.toStdString().c_str()];

	std::vector<int> ids;
	std::vector<QString> m_list;

	for (rapidjson::Value::ConstMemberIterator itr = model.MemberBegin();
		 itr != model.MemberEnd(); ++itr) {
		const QString key = QString::fromStdString(
			itr->name.GetString());

		int id = key.toInt();
		ids.push_back(id);

		const QString label = QString::fromStdString(
			itr->value["label"].GetString());
		m_list.push_back(label);
	}

	std::sort(ids.begin(), ids.end());

	QStringList qStringList;
	for (int id : ids) {
		qStringList << m_list[id];
	}

	QStringListModel* listModel = new QStringListModel(qStringList);
	return listModel;
}
