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

struct ItemIconsMap
{
    enum class IconType {
    valid,
    invalid,
    warning};
    
    const QIcon & getIcon(const IconType & aIconType){
        return _icons.at(aIconType);
    }

    private:
        std::map<IconType, QIcon> _icons;
};


class TreeItemFactory;
class PropertiesModel;
class TreeItem : public QTreeWidgetItem
{
    public:

        enum class Column {
        Label,
        Visible,
        Actor
        };

        enum class DataRole {
            PropertiesModel = Qt::UserRole + 1,
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
    
        TreeItem(QTreeWidget* aParent,
                     const QDomElement & aElement,
                     PropertiesModel* aPropModel,
                     const ItemTypes::Root& aRootType);

        TreeItem(QTreeWidgetItem* aParent,
                     const QDomElement & aElement,
                     PropertiesModel* aPropModel,
                     const ItemTypes::Sub& aSubType);

        // void setText(int column, const QString &text) override;



    private:

        friend class TreeItemFactory;

        QDomElement _element;

        const bool _isRoot; //TODO: Implement two classes sub and root insted of having this flag here
        bool _itemValid;
        bool _itemActive;

        const ItemTypes::Root _rootType;
        const ItemTypes::Sub _subType; 

        // bool addPropertiesModel();
        // bool checkModel();
        
        // void setIcon(IconType);
};

#endif