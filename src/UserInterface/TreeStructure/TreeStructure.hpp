/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef TREESTRUCTURE_HPP
#define TREESTRUCTURE_HPP

#include "PropertiesModel.hpp"
#include "TreeContextMenu.hpp"
#include "DocumentHandler.hpp"
#include "TreeItem.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <QDebug>
#include <QHeaderView>
#include <QMap>
#include <QSharedPointer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtXml/QDomElement>

#include <vtkLogger.h>

class TreeContextMenu;
class AddTreeItemCommand;
class RemoveTreeItemCommand;
class TreeCommandManager;
class TreeItemFactory;
class TreeItem;


namespace fs = std::filesystem;
/**
 * Represents a custom tree structure widget for displaying hierarchical data.
 *
 * This class extends QTreeWidget and provides functionality for writing data to XML,
 * loading geometry files, building base object representations, adding nodes and properties,
 * parsing default properties, reading default properties, and managing the tree structure.
 *
 * @param parent The parent widget to which this TreeStructure belongs.
 */
class TreeStructure : public QTreeWidget {
	Q_OBJECT
public:
	TreeStructure(QWidget* parent);
	~TreeStructure();

	void addExistingItem(TreeItem* itemToAdd, TreeItem* aParentItem);

	void addSubItem(TreeItem* aParentItem, const ItemTypes::Sub& aSubType);

	/**
	 * Remove QTreeWidgetItem from the TreeStructure
	 *
	 * @param itemToRemove pointer to QTreeWidgetItem that will be removed
	 *
	 * @returns None
	 */


	/**
	 * Open an editor in the Name column of a TreeWidgetItem
	 *
	 * @param item item to be renamed
	 *
	 * @returns None
	 */
	void renameItem(QTreeWidgetItem* item);

	// Container for handling content of columns
	enum class Column {
		Label,
		Visible,
		Actor
	};

	TreeItem* getRootItem(const ItemTypes::Root& aRootType);

	QList<TreeItem*> getSubItems(const ItemTypes::Sub& aRootType);
	
private:

	TreeItem* createSubItem(TreeItem* aParentItem, const ItemTypes::Sub& aSubType);
    friend class AddTreeItemCommand;
    friend class AddTreeItemCommand;

	void removeSubItem(TreeItem* itemToRemove);
    friend class RemoveTreeItemCommand;
    friend class RemoveTreeItemCommand;

	void deleteSubItem(TreeItem* itemToDelete);
	/**
	 * Context menu that appears when item in TreeStructure is right-clicked
	 */
	TreeContextMenu* _contextMenu;

	/**
	 * Pointer to document handler that stores data of TreeWidgetItems
	 */
	TreeItemFactory* _treeItemFactory;

	/**
	* Pointer to document handler that stores data of TreeWidgetItems
	*/


	// TreeCommandManager _commandManager;

	std::unordered_map<ItemTypes::Root, TreeItem*> _rootItems;

	std::unordered_map<ItemTypes::Sub, QList<TreeItem*>> _subItems;
};

#endif