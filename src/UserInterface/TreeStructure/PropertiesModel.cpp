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

#include "PropertiesModel.hpp"
#include "DocUtils.hpp"

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
PropertiesModel::PropertiesModel(const QDomElement& aPropertiesElement, QWidget* aParent)
    : QAbstractTableModel(aParent)
    , _element(aPropertiesElement){

    this->_header << "Property"
                  << "Value";

	QDomNodeList properties = aPropertiesElement.childNodes();
    for (int row = 0; row < properties.length(); ++row) {
        if (!properties.at(row).isComment()) {
            QDomElement propertyElement = properties.at(row).toElement();
            this->_properties.insert(row, propertyElement);
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
	return this->_properties.count();
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

	QDomElement propNode = this->_properties[index.row()];
	QVariant dataVariant;
	if (index.column() == PropertiesModel::Col::Label) {
		dataVariant = QVariant(Properties::getPropertyAttribute(propNode, QString("label")));
	} else if (index.column() == PropertiesModel::Col::Data) {
		dataVariant = propNode.firstChild().toText().data();
	}

	return dataVariant;
}

//--------------------------------------------------------------------------------------
bool PropertiesModel::setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) {
	if (!index.isValid() || role != Qt::DisplayRole && role != Qt::EditRole) {
		return false;
	}

	QDomElement property = this->_properties[index.row()];
	if (index.column() == PropertiesModel::Col::Data) {
		Properties::setPropertyValue(property, value.toString());
		emit dataChanged(index, index);
		return true;
	} else if (index.column() == PropertiesModel::Col::Label) {
		Properties::setPropertyAttribute(property, "label", value.toString());
	} else {
		qWarning() << "Invalid model index column.";
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

void PropertiesModel::setElementProperty(const QModelIndex& index, const QString& name, const QVariant& value) {
	QDomNodeList propertyNodes = _element.childNodes();
    for (int i = 0; i < propertyNodes.count(); ++i) {
        QDomElement propertyElement = propertyNodes.at(i).toElement();
        if (!propertyElement.isNull() && propertyElement.attribute("name") == name) {
            propertyElement.firstChild().setNodeValue(value.toString());
            break;
        }
    }
	emit dataChanged(index, index);
}

//--------------------------------------------------------------------------------------
QWidget* PropertiesModel::getWidget( const QModelIndex& aIndex, QWidget* aWidgetParent ) {
	if (!aIndex.isValid()) {
		qWarning("Widget index invalid!");
	}
	if(aIndex.column() != PropertiesModel::Col::Data){
		qWarning("Widget index should have column == 1");
	}

	QDomElement property = _properties[aIndex.row()];
	QString widgetName = Properties::getPropertyAttribute(property, "widget");
	BaseWidget* newWidget = nullptr;
	newWidget = WidgetFactory::createWidget(widgetName, aWidgetParent);
	if(newWidget){
		newWidget->setIndex(aIndex);
	} else {
		qWarning() << widgetName << " widget could not be added to PropertiesWidget";
	}
	
	return newWidget;
}
