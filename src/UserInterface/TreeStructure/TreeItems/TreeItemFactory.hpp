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

#ifndef TREEITEMFACTORY_HPP
#define TREEITEMFACTORY_HPP


#include "PropertiesModel.hpp"
#include "PropertiesWidget.hpp"
#include "TreeItem.hpp"
#include "DocItemTypes.hpp"
#include "DocumentHandler.hpp"

#include <QtXml/QDomElement>
#include <map>  

class TreeItem;
class TreeStructure;

class TreeItemFactory 
{
    public:
        TreeItemFactory(TreeStructure* aTreeStructure);
        ~TreeItemFactory() = default;
        TreeItemFactory(const TreeItemFactory &) = delete;

        TreeItem* createRootItem( const ItemTypes::Root & aRootItemType );

        TreeItem* createSubItem( TreeItem* aParentItem,
                                 const ItemTypes::Sub  & aSubItemType );

        QString getUniqueItemLabel( const ItemTypes::Sub & aSubType ) const;

    private:
    //TODO: Remove the dependency on _treeStructure and make the factory a namespace instead of class
        TreeStructure*  _treeStructure;
};



#endif