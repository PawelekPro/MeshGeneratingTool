/*
 * Copyright (C) 2024 Paweł Gilewicz
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

class TreeContextMenu;

#include "document.h"
#include "istreamwrapper.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <QDebug>
#include <QFile>
#include <QHeaderView>
#include <QMap>
#include <QPointer>
#include <QSharedPointer>
#include <QStringList>
#include <QTextStream>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include <vtkLogger.h>

namespace fs = std::filesystem;

/**
 * Defines a custom data structure for storing a list of properties.
 * Each property is represented as a map with key-value pairs.
 */
using PropertiesList = QList<QMap<QString, QString>>;

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
	 * Writes data to an XML file.
	 *
	 * @param data The data to be written to the XML file.
	 *
	 * @returns None
	 */
	void writeDataToXML(const std::string);

	/**
	 * Loads a geometry file using the specified file path.
	 *
	 * @param filePath The path to the geometry file to be loaded.
	 *
	 * @returns None
	 */
	void loadGeometryFile(const QString);
	/**
	 * Open an editor in the Name column of a TreeWidgetItem
	 *
	 * @param item item to be renamed
	 *
	 * @returns None
	 */
	void renameItem(QTreeWidgetItem* item);
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

	// Container for handling content of columns
	enum class Column {
		Label,
		Visible,
		Actor
	};


	/**
	 * Enum class representing different types of tree roots.
	 * TreeRoot can be one of the following:
	 * - GeomImport
	 * - GeomModel
	 * - CSYS
	 * - Mesh
	 */
	enum class TreeRoot {
		GeomImport,
		GeomModel,
		CSYS,
		Mesh
	};

	enum class XMLTag{
		MeshSizing, 
		GeometryImport,
	};
	QMap<XMLTag, QString> XMLTags = {
		{XMLTag::MeshSizing, "MeshSizing"},
		{XMLTag::GeometryImport, "GeometryImport"},
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
	 * Return PropertiesList of root node in the TreeStructure. Tree nodes are described using TreeRoot enum
	 * @param root
	 *
	 * @returns  PropertiesList of a tree root
	 */
	PropertiesList getRootProperties(TreeRoot root);

	/**
	 * Get Map of all instances of item described by itemTag, where key is the items QDomElement name
	 * and value is the PropertiesList of a given item
	 * @param itemTag xmlTag of items to be found
	 * @param root TreeRoot under which the items are placed
	 *
	 * @returns  Map of all instances of item described by itemTag, where key is the items QDomElement name
	 * and value is the PropertiesList of a given item
	 */
	QMap<QString, PropertiesList> getItemsProperties(TreeRoot root, XMLTag itemTag);

	TreeWidgetEventHandler* eventHandler;

private:
	void treeWidgetItemRenamed(QTreeWidgetItem* renamedItem,QString newName);

	/**
	 * @brief  Build base widget representation by adding roots item to the tree structure.
	 *
	 */
	void addRootItems();

	/**
	 * @brief  Create QTreeWidgetItem object and store reference to coresponding QDomElement.
	 *
	 * @param  {QSharedPointer<QDomElement>} element    : QDomElement object coresponding to tree item.
	 * @param  {QTreeWidgetItem*} parent : Parent item which will be the parent of the created item.
	 * @return {QTreeWidgetItem*}        : New QTreeWidgetItem object.
	 */
	QTreeWidgetItem* createTreeWidgetItem(const QDomElement&, QTreeWidgetItem* parent = nullptr);

	/**
	 * Finds tree widget items that match the specified text and flags.
	 *
	 * @param text The text to match against.
	 * @param flags The matching flags to use.
	 *
	 * @returns A list of QTreeWidgetItem pointers that match the search criteria.
	 */
	QList<QTreeWidgetItem*> findTreeWidgetItems(const QString&, Qt::MatchFlags);
	/**
	 * Changes the name in QDomElment tied to QTreeWidgetItem.
	 *
	 * @param itemToRename Pointer to QTreeWidgetItem that is being renamed
	 * @param newName New name assigned to QTReeWidgetItem
	 */
	void renameTreeWidgetItem(QTreeWidgetItem* itemToRename, const QString& newName);
	/**
	 * Finds tree widget items that match the specified text and flags.
	 *
	 * @param text The text to match against.
	 * @param flags The matching flags to use.
	 *
	 * @returns A list of QTreeWidgetItem pointers that match the search criteria.
	 */
	QTreeWidgetItem* getRootTreeWidgetItem(TreeRoot root);
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
	void addSubItem(QTreeWidgetItem* parentItem,  XMLTag xmlTag, const QString& nodeBaseName,
		Qt::ItemFlags flags = Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	/**
	 * Adds properties of a model to a tree widget item.
	 *
	 * @param modelElement Pointer to the QDomElement representing the model.
	 * @param treeItem Pointer to the QTreeWidgetItem where the properties will be added.
	 *
	 * @returns None
	 */
	void addPropertiesModel(const QDomElement&, QTreeWidgetItem*);

	/**
	 * Adds properties to a QDomElement.
	 *
	 * @param element Pointer to the QDomElement to which properties will be added.
	 * @param properties List of properties to add to the element.
	 *
	 * @returns None
	 */
	void addProperties(QDomElement&, PropertiesList);

	/**
	 * Parses the default properties from a RapidJSON document and a QString.
	 *
	 * @param document The RapidJSON document containing the properties.
	 * @param str The QString containing additional properties.
	 *
	 * @returns A PropertiesList object containing the parsed default properties.
	 */
	PropertiesList parseJsonDocument(const rapidjson::Document& document, QString prop);

	/**
	 * Reads and returns the default properties from a JSON document using RapidJSON.
	 *
	 * @return A RapidJSON Document containing the default properties.
	 */
	rapidjson::Document readJsonTemplateFile(const QString& jsonTemplatePath);

	/**
	 * QDomDocument object that represents an XML document object model.
	 */
	QDomDocument docObjectModel;

	/**
	 * QMap that maps QTreeWidgetItem pointers to QDomElements.
	 */
	QMap<QTreeWidgetItem*, QDomElement> domElements;

	/**
	 * Context menu that appears when item in TreeStructure is right-clicked
	 */
	TreeContextMenu* contextMenu;


	/**
	 * Generates new names for items created under the parentItem. Follows the pattern baseName_1, baseName_2..
	 */
	QString getUniqueElementNameForTag(QTreeWidgetItem* parentItem, XMLTag tag, const QString& baseName);

	/**
	 * A QMap that maps TreeRoot enum values to corresponding QString descriptions.
	 *
	 * TreeRoot::GeomImport - "Geometry Imports"
	 * TreeRoot::GeomModel - "Geometry Model"
	 * TreeRoot::CSYS - "Coordinate System"
	 * TreeRoot::Mesh - "Mesh"
	 */
	const QMap<TreeRoot, QString> TreeRoots {
		{ TreeRoot::GeomImport, "Geometry Imports" },
		{ TreeRoot::GeomModel, "Geometry Model" },
		{ TreeRoot::CSYS, "Coordinate System" },
		{ TreeRoot::Mesh, "Mesh" }
	};

	private slots:
		void onItemChanged(QTreeWidgetItem* item, int column);	
};

#endif