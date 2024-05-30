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

#include "PropertiesModel.h"

QMap<QString, std::function<BaseWidget*()>> PropertiesModel::factoryMap = {
	{ "IntLineWidget", createInstance<IntLineWidget> },
	{ "DoubleLineWidget", createInstance<DoubleLineWidget> },
	{ "ComboBoxWidget", createInstance<ComboBoxWidget> },
	{ "EntityPickWidget", createInstance<EntityPickWidget> }
};

//--------------------------------------------------------------------------------------
bool ModelFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {
	Q_UNUSED(sourceParent);

	QAbstractItemModel* model = this->sourceModel();
	model->data(model->index(sourceRow, 0), Qt::DisplayRole);

	PropertiesModel* propsModel = dynamic_cast<PropertiesModel*>(model);

	QDomElement propsNode = propsModel->getProperty(sourceRow);
	QString hidden = propsNode.attributes().namedItem("hidden").toAttr().value();

	return hidden == "no" || hidden.isEmpty();
}

//--------------------------------------------------------------------------------------
PropertiesModel::PropertiesModel(const QDomElement& element, QWidget* parent)
    : QAbstractTableModel(parent)
    , _element(element) {

    this->_header << "Property"
                  << "Value";

    QDomNodeList props = this->_element.childNodes();

    for (int i = 0; i < props.length(); ++i) {
        if (!props.at(i).isComment()) {
            QDomElement propertyElement = props.at(i).toElement();
            QString propertyName = propertyElement.tagName();
            QString propertyValue = propertyElement.text();
            this->_properties.append(propertyElement);
        }
    }
	// connect(this, &PropertiesModel::dataChanged, this, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
    //         QDomNodeList list = this->_element.childNodes();
    //         for (int i = 0; i < list.length(); ++i) {
    //             QDomNode node = list.at(i);
    //             if (node.isElement()) {
    //                 QDomElement elem = node.toElement();
    //                 std::cout << "Element: " << elem.tagName().toStdString() << ", Value: " << elem.text().toStdString() << std::endl;
    //             }
    //         }
    //     });
}

//--------------------------------------------------------------------------------------
PropertiesModel::~PropertiesModel() { }

//--------------------------------------------------------------------------------------
int PropertiesModel::rowCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return this->_properties.length();
}

//--------------------------------------------------------------------------------------
int PropertiesModel::columnCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return 2;
}

//--------------------------------------------------------------------------------------
QVariant PropertiesModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	QDomElement propNode = this->_properties.value(index.row());
	QVariant ret;
	if (index.column() == 0) {
		QDomAttr attr = propNode.attributes().namedItem("label").toAttr();
		ret = attr.value();
	} else if (index.column() == 1) {
		ret = propNode.firstChild().toText().data();
	}

	return ret;
}

//--------------------------------------------------------------------------------------
bool PropertiesModel::setData(
	const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) {
	if (!index.isValid() || role != Qt::DisplayRole && role != Qt::EditRole) {
		return false;
	}

	QDomElement propNode = this->_properties.value(index.row());
	if (index.column() == 1) {
		QDomText textNode = propNode.firstChild().toText();
		if (textNode.isNull()) {
			QDomDocument doc = propNode.ownerDocument();
			QDomText newText = doc.createTextNode(value.toString());
			propNode.appendChild(newText);
		} else {
			textNode.setData(value.toString());
		}

		emit dataChanged(index, index);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
QVariant PropertiesModel::headerData(
	int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			return this->_header.value(section);
		}
	}
	return QVariant();
}

//--------------------------------------------------------------------------------------
Qt::ItemFlags PropertiesModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags _flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (index.column() == 1) {
		_flags |= Qt::ItemIsEditable;
	}
	return _flags;
}

//--------------------------------------------------------------------------------------
const QDomElement PropertiesModel::getProperty(int row) {
	if (row < 0 || row >= _properties.size()) {
		throw std::out_of_range("Invalid row index");
	}
	return this->_properties.value(row);
}

//--------------------------------------------------------------------------------------
QWidget* PropertiesModel::getWidget(const QModelIndex& index) {
	if (!index.isValid()) {
		return nullptr;
	}

	QWidget* widget = nullptr;
	if (index.column() == 1) {
		QDomNamedNodeMap attrs = this->_properties[index.row()].attributes();
		QString name = attrs.namedItem("widget").toAttr().value();

		auto classType = this->factoryMap.find(name);
		if (classType != this->factoryMap.end()) {
			auto createFunction = classType.value();
			auto widget = createFunction(); // Call the std::function to create the widget

			widget->setIndex(index);
			return widget;
		} else {
			vtkLogF(
				ERROR, ("Class not found in widgets factory map: " + name.toStdString()).c_str());
		}
	}
	return widget;
}