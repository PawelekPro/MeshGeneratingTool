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
#include "PropertiesModel.h"
#include "TreeContextMenu.h"
#include "TreeWidgetEventHandler.h"
#include "DocumentHandler.h"

class TreeContextMenu;


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <QDebug>
#include <QHeaderView>
#include <QMap>
#include <QSharedPointer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtXml/QDomElement>

#include <vtkLogger.h>

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
	 * Initializes TreeStructure with QDomDocument Handler, builds root items
	 * @returns None
	 */
	void initialize(DocumentHandler* DocumentHandler);
	/**
	 * Loads a geometry file using the specified file path.
	 *
	 * @param filePath The path to the geometry file to be loaded.
	 *
	 * @returns None
	 */
	void loadGeometryFile(const QString& filePath);
	/**
	 * @brief  mesh sizing node and creates its input widgets in the tree.
	 *
	 */
	void addMeshSizing();
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
	
	/**
	 * Event handler for widgets inside Tree Widget Items.
	 */
	TreeWidgetEventHandler* _eventHandler;


private:
	QMap<DocumentHandler::RootTag, QString> rootLabels = {
		{DocumentHandler::RootTag::GeomModel, "Geometry model"},
		{DocumentHandler::RootTag::GeomImport, "Geometry imports"},
		{DocumentHandler::RootTag::CSYS, "CSYS"},
		{DocumentHandler::RootTag::Mesh, "Mesh"}
	};
	/**
	 * @brief  Build base widget representation by adding roots item to the tree structure.
	 */
	void addRootItem(const DocumentHandler::RootTag& rootTag);
	/**
	 * Adds a QTreeWidget item to TreeStructue and its QDomElement to main xml document
	 *
	 * @param parentItem Parent item of the newly created one.
	 * @param xmlTag xml tag enum that will be used for marking the item in the xml and for parsing its proerties
	 *  in the DefaultProperties.json
	 * @param nodeBaseName base name for inserted name, folliwng names will be created with 1,2,... suffix
	 * @param flags Flags of the newly created  QTreeWidget item. By default they are set to: 
	 * Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled
	 * @returns None
	 */
	void addSubItem(QTreeWidgetItem* parentItem,  const DocumentHandler::EntryTag&, const QString& itemBaseName,
		Qt::ItemFlags flags = Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	/**
	 * @brief  Create QTreeWidgetItem object and store reference to coresponding QDomElement.
	 *
	 * @param  {QDomElement} element    : QDomElement object coresponding to tree item.
	 * @param  {QTreeWidgetItem*} parent : Parent item which will be the parent of the created item.
	 * @return {QTreeWidgetItem*}        : New QTreeWidgetItem object.
	 */
	QTreeWidgetItem* createTreeWidgetItem(const QDomElement&, QTreeWidgetItem* parent = nullptr);

	/**
	 * @brief  Creates instance of PropertiesModel based on the QDomElement and adds it to QTreeWidgetItem
	 *
	 * @param  {QDomElement} element    : QDomElement based on which the PropertiesModel will be created
	 * @param  {QTreeWidgetItem*} parent : Item to which the PropertiesModel will be added
	 */
	void addPropertiesModel(const QDomElement& element, QTreeWidgetItem* item);

	/**
	 * @param  {QTreeWidgetItem} parentItem : parentItem under which to search for existing names
	 * @param  {DocumentHandler::EntryTag} entry tag : Entry tag family that will be searched for existing names
	 * @return {QString}        : Unique name following pattern of baseName_1, baseName_2, ...
	 */
	QString getUniqueElementNameForTag(QTreeWidgetItem* parentItem,
										const DocumentHandler::EntryTag& entryTag,
										const QString& baseName);
	/**
	 * Changes the name in QDomElment tied to QTreeWidgetItem.
	 *
	 * @param itemToRename Pointer to QTreeWidgetItem that is being renamed
	 * @param newName New name assigned to QTReeWidgetItem
	 */
	void treeWidgetItemRenamed(QTreeWidgetItem* renamedItem,QString newName);

	/**
	 * @return QTreeWidgetItem described by the rootTag from DocumentHandler
	 */
	QTreeWidgetItem* getRootTreeWidgetItem(const DocumentHandler::RootTag& rootTag);

	/**
	 * QMap that maps QTreeWidgetItem pointers to QDomElements.
	 */
	QMap<QTreeWidgetItem*, QDomElement> _domElements;

	/**
	 * Context menu that appears when item in TreeStructure is right-clicked
	 */
	TreeContextMenu* _contextMenu;

	/**
	 * Pointer to document handler that stores data of TreeWidgetItems
	 */
	DocumentHandler* _documentHandler;

	private slots:
		void onItemRenamed(QTreeWidgetItem* item, int column);	
};

#endif