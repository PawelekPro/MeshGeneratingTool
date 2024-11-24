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

#include "TreeItem.hpp"

TreeItem::TreeItem( QTreeWidget* aParent,
                            const QDomElement& aElement,
                            PropertiesModel* aPropModel,
                            const ItemTypes::Root& aRootType) : 
                            QTreeWidgetItem(aParent),
                            _element(aElement),
                            _propModel(aPropModel),
                            _rootType(aRootType),
                            _isRoot(true)
                            {} 

TreeItem::TreeItem( QTreeWidgetItem* aParent,
                            const QDomElement& aElement,
                            PropertiesModel* aPropModel,
                            const ItemTypes::Sub & aSubType) : 
                            QTreeWidgetItem(aParent),
                            _element(aElement),
                            _propModel(aPropModel),
                            _subType(aSubType),
                            _rootType(ItemTypes::rootType(aSubType)),
                            _isRoot(false)
                            {} 

TreeItem::~TreeItem(){}

// void TreeItem::setText(int column, const QString &text){
//   QTreeWidgetItem::setText(column, text);
//     if (!_element.isNull()) {
//         DocumentHandler::setNameProperty(_element, text);
//     }
// }
      