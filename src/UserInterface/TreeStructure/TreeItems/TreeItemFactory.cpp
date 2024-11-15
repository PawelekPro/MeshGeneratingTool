/*
 * Copyright (C) 2024 Krystian Fudali
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

#include "TreeItemFactory.hpp"
#include <utility>

TreeItemFactory::TreeItemFactory(TreeStructure* aTreeStructure) : 
                                 _treeStructure(aTreeStructure){};


TreeItem* TreeItemFactory::createRootItem (const ItemTypes::Root & aRootItemType) {

    DocumentHandler& docHandler = DocumentHandler::getInstance();
    QDomElement element =  docHandler.createRootElement(aRootItemType);
    QDomElement propertiesElement = DocumentHandler::getProperties(element);

    PropertiesModel* propertiesModel = new PropertiesModel(propertiesElement, _treeStructure);
    TreeItem* newRootItem = new TreeItem(_treeStructure, element, propertiesModel, aRootItemType);
    newRootItem->setData(0, static_cast<int>(TreeItem::DataRole::PropertiesModel), QVariant::fromValue(propertiesModel));

    QVariant testVariant = newRootItem->data(0, Qt::UserRole + 1);
    PropertiesModel* testModel = testVariant.value<PropertiesModel*>();

    newRootItem->setText(static_cast<int>(TreeStructure::Column::Label), ItemTypes::label(aRootItemType));
   
    return newRootItem;
}

TreeItem* TreeItemFactory::createSubItem (TreeItem* aParentItem, const ItemTypes::Sub & aSubItemType) {

    DocumentHandler& docHandler = DocumentHandler::getInstance();
    QDomElement rootElement = docHandler.getRootElement(ItemTypes::rootType(aSubItemType));
    QDomElement element = docHandler.createSubElement(aSubItemType, rootElement);
    QDomElement propertiesElement = DocumentHandler::getProperties(element);

    PropertiesModel* propertiesModel = new PropertiesModel(propertiesElement, _treeStructure);
    TreeItem* newSubItem = new TreeItem(aParentItem, element, propertiesModel, aSubItemType);

    QString uniqueLabel = getUniqueItemLabel(aSubItemType);
    
    newSubItem->setText(static_cast<int>(TreeItem::Column::Label), uniqueLabel);
    newSubItem->setData(0, Qt::UserRole + 1, QVariant::fromValue(propertiesModel));

    return newSubItem;
}

QString TreeItemFactory::getUniqueItemLabel( const ItemTypes::Sub & aSubType ) const {

    QString baseLabel = ItemTypes::label(aSubType);

    QList<TreeItem*> itemList = _treeStructure->getSubItems(aSubType);
    if (itemList.empty()){
        return baseLabel;
    }
    int maxLabelNum = 1;
    int numToAppend = std::accumulate(itemList.begin(), itemList.end(), maxLabelNum, 
    [baseLabel](int currentMax, const TreeItem* item) {
        std::string label = item->text(static_cast<int>(TreeItem::Column::Label)).toStdString();
        if (label.rfind(baseLabel.toStdString(), 0) == 0) { 
            size_t baseNameLength = baseLabel.length();
            if (label.length() > baseNameLength && label[baseNameLength] == '-') {
                std::string numStr = label.substr(baseNameLength + 1);
                if (!numStr.empty() && std::all_of(numStr.begin(), numStr.end(), ::isdigit)) {
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
