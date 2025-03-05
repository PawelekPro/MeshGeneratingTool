/*
 * Copyright (C) 2024 Krystian Fudali
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

#include "TreeItemFactory.hpp"
#include "PropertiesWidget.hpp"
#include "TreeStructure.hpp"

#include <utility>

#include <QApplication>

TreeItemFactory::TreeItemFactory(TreeStructure* aTreeStructure)
	: _treeStructure(aTreeStructure) { };

TreeItem* TreeItemFactory::createItemImportSTEP(const QString& aFilePath) {

	TreeItem* geometryItem
		= _treeStructure->getRootItem(ItemTypes::Root::Geometry);
	TreeItem* importItem
		= createSubItem(geometryItem, ItemTypes::Geometry::ImportSTEP);
	QDomElement element = importItem->getElement();
	QDomElement properties = DocUtils::getSubElement(element, "Properties");
	QDomElement importProperty
		= Properties::getProperty(properties, "stepImport");
	Properties::setPropertyValue(importProperty, aFilePath);
	return importItem;
}

TreeItem* TreeItemFactory::createItemElementSizing(
	const std::vector<int>& aShapesTags,
	const IVtk_SelectionMode& aSelectionType) {
	TreeItem* meshItem = _treeStructure->getRootItem(ItemTypes::Root::Mesh);
	TreeItem* sizingItem
		= createSubItem(meshItem, ItemTypes::Mesh::ElementSizing);
	QDomElement element = sizingItem->getElement();
	QString shapesTagsString = DocUtils::intsToString(aShapesTags);

	QDomElement properties = DocUtils::getSubElement(element, "Properties");
	// TODO: add some doc handler setPropertyValue that would set the text node
	QDomElement selectedTagsProperty
		= Properties::getProperty(properties, "selectedTags");
	Properties::setPropertyValue(selectedTagsProperty, shapesTagsString);

	QString selectionTypeString;
	switch (aSelectionType) {
	case SM_Shape: {
		selectionTypeString = "Shape";
		break;
	}
	case SM_Vertex: {
		selectionTypeString = "Vertex";
		break;
	}
	case SM_Edge: {
		selectionTypeString = "Edge";
		break;
	}
	case SM_Face: {
		selectionTypeString = "Face";
		break;
	}
	default: {
		selectionTypeString = "Unknown";
		break;
	}
	}

	QDomElement selectionTypeProperty
		= Properties::getProperty(element, "selectionType");
	Properties::setPropertyValue(selectionTypeProperty, selectionTypeString);

	return sizingItem;
}

TreeItem* TreeItemFactory::createRootItem(
	const ItemTypes::Root& aRootItemType) {

	DocumentHandler& docHandler = DocumentHandler::getInstance();
	QDomElement element = docHandler.createRootElement(aRootItemType);

	QDomElement properties = DocUtils::getSubElement(element, "Properties");

	PropertiesModel* propertiesModel
		= new PropertiesModel(properties, _treeStructure);
	TreeItem* newRootItem
		= new TreeItem(_treeStructure, element, propertiesModel, aRootItemType);
	newRootItem->setData(0,
		static_cast<int>(TreeItem::DataRole::PropertiesModel),
		QVariant::fromValue(propertiesModel));

	QVariant testVariant = newRootItem->data(0, Qt::UserRole + 1);
	newRootItem->setText(static_cast<int>(TreeStructure::Column::Label),
		ItemTypes::label(aRootItemType));

	auto propertiesWidget
		= qApp->property("PropertiesWidget").value<PropertiesWidget*>();
	QObject::connect(propertiesModel, &PropertiesModel::modelDataChanged,
		propertiesWidget, &PropertiesWidget::onModelDataChanged,
		Qt::UniqueConnection);

	return newRootItem;
}

TreeItem* TreeItemFactory::createSubItem(
	TreeItem* aParentItem, const ItemTypes::Sub& aSubItemType) {

	DocumentHandler& docHandler = DocumentHandler::getInstance();
	QDomElement rootElement
		= docHandler.getRootElement(ItemTypes::rootType(aSubItemType));
	QDomElement element
		= docHandler.createSubElement(aSubItemType, rootElement);
	QDomElement properties = DocUtils::getSubElement(element, "Properties");

	PropertiesModel* propertiesModel
		= new PropertiesModel(properties, _treeStructure);
	TreeItem* newSubItem
		= new TreeItem(aParentItem, element, propertiesModel, aSubItemType);

	QString uniqueLabel = getUniqueItemLabel(aSubItemType);

	newSubItem->setText(static_cast<int>(TreeItem::Column::Label), uniqueLabel);
	newSubItem->setData(
		0, Qt::UserRole + 1, QVariant::fromValue(propertiesModel));

	return newSubItem;
}

QString TreeItemFactory::getUniqueItemLabel(
	const ItemTypes::Sub& aSubType) const {

	QString baseLabel = ItemTypes::label(aSubType);

	QList<TreeItem*> itemList = _treeStructure->getSubItems(aSubType);
	if (itemList.empty()) {
		return baseLabel;
	}
	int maxLabelNum = 1;
	int numToAppend = std::accumulate(itemList.begin(), itemList.end(),
		maxLabelNum, [baseLabel](int currentMax, const TreeItem* item) {
			std::string label
				= item->text(static_cast<int>(TreeItem::Column::Label))
					  .toStdString();
			if (label.rfind(baseLabel.toStdString(), 0) == 0) {
				size_t baseNameLength = baseLabel.length();
				if (label.length() > baseNameLength
					&& label[baseNameLength] == '-') {
					std::string numStr = label.substr(baseNameLength + 1);
					if (!numStr.empty()
						&& std::all_of(
							numStr.begin(), numStr.end(), ::isdigit)) {
						int labelNum = std::stoi(numStr);
						return std::max(labelNum, currentMax);
					}
				}
			}
			return currentMax;
		});
	QString uniqueLabel = baseLabel + "-" + QString::number(numToAppend);
	return uniqueLabel;
};
