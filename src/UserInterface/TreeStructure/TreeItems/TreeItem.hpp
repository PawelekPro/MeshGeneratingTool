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

#ifndef TREEITEM_HPP
#define TREEITEM_HPP

#include <QTreeWidgetItem>
#include <QtXml/QDomElement>

#include "DocItemTypes.hpp" 

#include <map>  

// TODO: implement some sort of icon storage - can be in other class
// struct ItemIconsMap
// {
//     enum class IconType {
//     valid,
//     invalid,
//     warning};
    
//     const QIcon & getIcon(const IconType & aIconType){
//         return _icons.at(aIconType);
//     }

//     private:
//         std::map<IconType, QIcon> _icons;
// };


class TreeItemFactory;
class PropertiesModel;
class TreeItem : public QTreeWidgetItem
{
    public:
	/**
	 * @brief Enum with labels of TreeItem columns
	 * 		  -Label: name of the tree item
	 * 		  -Visible: icon/text to mark whether item is visible
	 * 		  -Actor: placeholder for any icon/additional label
	 */
    enum class Column {
    Label,
    Visible,
    Actor
    };

	/**
	 * @brief Enum with custom Qt::Roles
	 * 		  -PropertiesModel: role under which the Items PropetiesModel is stored.
	 */
    enum class DataRole {
        PropertiesModel = Qt::UserRole + 1, //TODO: add another role for QDomElement and switch all ugly getElement to getData(role)
    };

    virtual ~TreeItem();
    TreeItem(const TreeItem& item) = delete;

    inline bool isActive(){return _itemActive;};
    inline bool isValid(){return _itemValid;};

    inline QDomElement getElement(){return _element;};

    const ItemTypes::Root & rootType(){return _rootType;};
    const ItemTypes::Sub & subType(){return _subType;};
    const bool & isRoot(){return _isRoot;}

    PropertiesModel*  _propModel;

    protected:

    /**
     * @brief Creates a TreeItem with ItemType set to Root.
     * 
     * @param aParent Pointer to the parent QTreeWidget that will contain this TreeItem.
     * @param aElement A QDomElement representing the associated XML data for this TreeItem.
     * @param aPropModel Pointer to the PropertiesModel containing metadata or settings for this TreeItem.
     * @param aRootType The type of the root item, defined in ItemTypes::Root.
     */
    TreeItem(QTreeWidget* aParent,
            const QDomElement& aElement,
            PropertiesModel* aPropModel,
            const ItemTypes::Root& aRootType);

    /**
     * @brief Creates a TreeItem with ItemType set to Sub.
     * 
     * @param aParent Pointer to the parent QTreeWidgetItem that will contain this TreeItem as a child.
     * @param aElement A QDomElement representing the associated XML data for this TreeItem.
     * @param aPropModel Pointer to the PropertiesModel containing metadata or settings for this TreeItem.
     * @param aSubType The type of the sub-item, defined in ItemTypes::Sub.
     */
    TreeItem(QTreeWidgetItem* aParent,
            const QDomElement& aElement,
            PropertiesModel* aPropModel,
            const ItemTypes::Sub& aSubType);
    private:

        friend class TreeItemFactory;

        QDomElement _element;

        const bool _isRoot; //TODO: Implement two classes sub and root insted of having this flag here
        bool _itemValid;
        bool _itemActive;

        const ItemTypes::Root _rootType;
        const ItemTypes::Sub _subType; 
};

#endif