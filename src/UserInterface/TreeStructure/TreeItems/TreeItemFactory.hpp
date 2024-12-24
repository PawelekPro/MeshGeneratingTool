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
#include "IVtk_Types.hxx"

#include <QtXml/QDomElement>
#include <map>  

class TreeItem;
class TreeStructure;

/**
 * @brief Factory class for creating various types of TreeItem instances.
 * 
 * The TreeItemFactory provides methods to create root and sub-items,
 * as well as specialized items like ImportSTEP and ElementSizing items.
 * 
 * @note This class currently depends on TreeStructure, but it could be refactored into a namespace
 *       to remove this dependency.
 */
class TreeItemFactory 
{
public:
    /**
     * @brief Constructs a TreeItemFactory with a reference to the TreeStructure.
     * 
     * @param aTreeStructure Pointer to the TreeStructure object used for item creation.
     */
    TreeItemFactory(TreeStructure* aTreeStructure);
    ~TreeItemFactory() = default;
    TreeItemFactory(const TreeItemFactory &) = delete;

    /**
     * @brief Creates a root item for the TreeStructure.
     * 
     * @param aRootItemType The type of root item to create, defined in ItemTypes::Root.
     * @return Pointer to the newly created root TreeItem.
     */
    TreeItem* createRootItem(const ItemTypes::Root & aRootItemType);

    /**
     * @brief Creates a sub-item under a specified parent item. 
     * 
     * @param aParentItem Pointer to the parent TreeItem to which the sub-item will be added.
     * @param aSubItemType The type of sub-item to create, defined in ItemTypes::Sub.
     * @return Pointer to the newly created sub-item TreeItem.
     */
    TreeItem* createSubItem(TreeItem* aParentItem, const ItemTypes::Sub & aSubItemType);

    /**
     * @brief Creates an item representing an imported STEP file.
     * 
     * @param aFilePath The file path of the STEP file to be imported.
     * @return Pointer to the newly created TreeItem for the imported STEP file.
     */
    TreeItem* createItemImportSTEP(const QString& aFilePath);

    /**
     * @brief Creates an item representing element sizing information.
     * 
     * @param aShapesTags A vector of integers representing the tags of the shapes to be sized.
     * @param aSelectionType The selection mode specifying how the shapes are considered.
     * @return Pointer to the newly created TreeItem for element sizing.
     */
    TreeItem* createItemElementSizing(const std::vector<int>& aShapesTags, const IVtk_SelectionMode& aSelectionType);

    /**
     * @brief Generates a unique label for a sub-item based on its type.
     * 
     * @param aSubType The type of sub-item for which a unique label is to be generated.
     * @return A QString containing the unique label.
     */
    QString getUniqueItemLabel(const ItemTypes::Sub & aSubType) const;

private:
    TreeStructure* _treeStructure;
};

#endif