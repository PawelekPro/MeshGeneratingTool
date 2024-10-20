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

#ifndef TREESTRUCTURE_H
#define TREESTRUCTURE_H

#include "AppDefaults.h"
#include "PropertiesModel.hpp"
#include "TreeContextMenu.hpp"
#include "DocumentHandler.hpp"
#include "TreeItemFactory.hpp"
#include "TreeCommand.hpp"

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

	/**
	 * Remove QTreeWidgetItem from the TreeStructure
	 *
	 * @param itemToRemove pointer to QTreeWidgetItem that will be removed
	 *
	 * @returns None
	 */

	void removeSubItem(QTreeWidgetItem* itemToRemove);
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

	/**
	 * Mapping of role names to corresponding Qt user roles.
	 */
	static const inline QMap<QString, int> Role = {
		{ "Properties", Qt::UserRole + 1 },
		// { "Advanced", Qt::UserRole + 2 },
		// { "References", Qt::UserRole + 3 },
		// { "HiddenText", Qt::UserRole + 4 },
		// { "Actor", Qt::UserRole + 6 }
	};


	TreeItem* getRootItem(const ItemTypes::Root& aRootType);

	QList<TreeItem*> getSubItems(const ItemTypes::Sub& aRootType);
	
private:

	/**
	 * Context menu that appears when item in TreeStructure is right-clicked
	 */
	TreeContextMenu* _contextMenu;

	/**
	 * Pointer to document handler that stores data of TreeWidgetItems
	 */
	TreeItemFactory* _treeItemFactory;

	const std::unordered_map<ItemTypes::Root, TreeItem*> _rootItems;

	std::unordered_map<ItemTypes::Sub, QList<TreeItem*>> _subItems;
};

#endif