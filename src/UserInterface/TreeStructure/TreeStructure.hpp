/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef TREESTRUCTURE_HPP
#define TREESTRUCTURE_HPP

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

#include "IVtk_Types.hxx"
#include <vtkLogger.h>

class TreeContextMenu;
class TreeItemFactory;
class ModelActionsHandler;

/**
 * Represents a custom tree structure widget for displaying hierarchical data.
 *
 * This class extends QTreeWidget and provides functionality for writing data to
 * XML, loading geometry files, building base object representations, adding
 * nodes and properties, parsing default properties, reading default properties,
 * and managing the tree structure.
 *
 * @param parent The parent widget to which this TreeStructure belongs.
 */
class TreeStructure : public QTreeWidget {
	Q_OBJECT
public:
	/**
	 * @brief Enum with labels of TreeItem columns
	 * 		  -Label: name of the tree item
	 * 		  -Visible: icon/text to mark whether item is visible
	 * 		  -Actor: placeholder for any icon/additional label
	 */
	using Column = TreeItem::Column;

	TreeStructure(QWidget* parent = nullptr);
	~TreeStructure();

	TreeStructure(const TreeStructure& aOther) = delete;
	TreeStructure& operator=(const TreeStructure& aOther) = delete;

	void Initialize();

	/**
	 * @brief Creates a sub-item under specified parent TreeItem based on the
	 * passed ItemType.
	 *
	 * @param aParentItem Pointer to the parent TreeItem to which the sub-item
	 * will be added.
	 * @param aSubType The type of sub-item to add, defined in ItemTypes::Sub.
	 * @return Pointer to the newly added TreeItem representing the sub-item.
	 */
	TreeItem* addSubItem(TreeItem* aParentItem, const ItemTypes::Sub& aSubType);
	/**
	 * @brief Creates a ImportSTEP item under Geometry root item.
	 *
	 * @param aFilePath - file path string that will be displayed in
	 * corresponding widget. and stored in created QDomElement associated with
	 * new ImportSTEP item.
	 * @return Pointer to the newly added TreeItem representing the ImportSTEP
	 * item.
	 */
	TreeItem* addImportSTEPItem(const QString& aFilePath);

	/**
	 * @brief Creates a ElementSizing item under Mesh root item.
	 *
	 * @param aShapesTags - tags of shapes that will be affected by element
	 * sizing.
	 * @param aSelectionType - type of shapes that will be affected by element
	 * siizng.
	 * @return Pointer to the newly added TreeItem representing the ImportSTEP
	 * item.
	 */
	TreeItem* addElementSizingItem(const std::vector<int>& aShapesTags,
		const IVtk_SelectionMode& aSelectionType);

	/**
	 * @brief Removes the TreeItem from TreeWidget without deleting it. This
	 * takes away ownership, from TreeStructure - possible memory leak if the
	 * item will not be explicitly deleted.
	 *
	 * @param aItemToRemove - TreeItem* to be removed from tree.
	 */
	void removeSubItem(TreeItem* aItemToRemove);

	/**
	 * @brief Removes TreeItem from TreeStructure and calls its destructor.
	 *
	 * @param aShapesTags - tags of shapes that will be affected by element
	 * sizing.
	 * @param aSelectionType - type of shapes that will be affected by element
	 * siizng.
	 * @return Pointer to the newly added TreeItem representing the ImportSTEP
	 * item.
	 */
	void deleteSubItem(TreeItem* itemToDelete);

	/**
	 * @brief Adds existing TreeItem to the TreeStructure under given parent
	 * item.
	 *
	 * @param aItemToAdd - TreeItem* to be added to the tree.
	 * @param aParentItem - parent under which the TreeItem* will be added.
	 */
	void addExistingItem(TreeItem* aItemToAdd, TreeItem* aParentItem);

	/**
	 * @brief Opens an edit line in place of QTreeWidgetItem label
	 *
	 * @param item - pointer to item that will trigger its editing field.
	 */
	void renameItem(QTreeWidgetItem* item);

	/**
	 * @brief returns the root TreeItem based on the spcified ItemTypes::Root
	 * enum.
	 *
	 * @param aRootType - ItemTypes::Root enum.
	 * @return Pointer to the root TreeItem.
	 */
	TreeItem* getRootItem(const ItemTypes::Root& aRootType);

	/**
	 * @brief Returns a QList collection of all sub TreeItems with specified
	 * ItemTypes::Sub type.
	 *
	 * @param aSubType - ItemTypes::Sub enum,
	 * @return QList with all TreeItem* of type aSubType that are currently
	 * children to TreeStructure.
	 */
	QList<TreeItem*> getSubItems(const ItemTypes::Sub& aSubType);

	/**
	 * @brief ModelActionsHandler setter.
	 */
	void setModelHandler(ModelActionsHandler* aModelHandler);

	/* getters */
	ModelActionsHandler* modelHandler() { return _modelHandler; };

private:
	TreeContextMenu* _contextMenu;
	TreeItemFactory* _treeItemFactory;
	ModelActionsHandler* _modelHandler;

	std::unordered_map<ItemTypes::Root, TreeItem*> _rootItems;
	std::unordered_map<ItemTypes::Sub, QList<TreeItem*>> _subItems;
};

#endif