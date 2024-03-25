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

#include "AppDefaults.hpp"
#include "PropertiesModel.h"

#include "document.h"
#include "istreamwrapper.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QHeaderView>
#include <QMap>
#include <QPointer>
#include <QSharedPointer>
#include <QStringList>
#include <QTextStream>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <vtkLogger.h>

namespace fs = std::filesystem;

class TreeStructure : public QTreeWidget {
	Q_OBJECT
public:
	TreeStructure(QWidget* parent);
	~TreeStructure();

	void writeDataToXML(const std::string);

	void loadGeometryFile(const QString);

	// Container for handling content of columns
	enum class Column {
		Label,
		Visible,
		Actor
	};

	QMap<QString, int> Role = {
		{ "Properties", Qt::UserRole + 1 },
		{ "Advanced", Qt::UserRole + 2 },
		{ "References", Qt::UserRole + 3 },
		{ "HiddenText", Qt::UserRole + 4 },
		{ "Actor", Qt::UserRole + 6 }
	};

private:
	/**
	 * @brief  Build base widget representation by adding roots item to the tree structure.
	 *
	 */
	void buildBaseObjectsRepresentation();

	/**
	 * @brief  Create QTreeWidgetItem object and store reference to coresponding QDomElement.
	 *
	 * @param  {QDomElement*} element    : QDomElement object coresponding to tree item.
	 * @param  {QTreeWidgetItem*} parent : Parent item which will be the parent of the created item.
	 * @return {QTreeWidgetItem*}        : New QTreeWidgetItem object.
	 */
	QTreeWidgetItem* createItem(QDomElement* element, QTreeWidgetItem* parent = nullptr);

	QList<QTreeWidgetItem*> findTreeWidgetItems(const QString&, Qt::MatchFlags);

	void addNode(const QString&, const QString&);

	void addPropertiesModel(QDomElement*, QTreeWidgetItem*);

	void parseDefaultProperties();

	rapidjson::Document readDefaultProperties();

	QDomDocument* docObjectModel;

	QMap<QTreeWidgetItem*, QDomElement*> domElements;

	enum class TreeRoot {
		GeomImport,
		GeomModel,
		CSYS,
		Mesh
	};

	const QMap<TreeRoot, QString> TreeRoots {
		{ TreeRoot::GeomImport, "Geometry Imports" },
		{ TreeRoot::GeomModel, "Geometry Model" },
		{ TreeRoot::CSYS, "Coordinate System" },
		{ TreeRoot::Mesh, "Mesh" }
	};

	// Handy-dandy container for application defaults
	AppDefaults appInfo;
};

#endif