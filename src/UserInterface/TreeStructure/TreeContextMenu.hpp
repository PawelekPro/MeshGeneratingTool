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

#ifndef TREECONTEXTMENU_HPP
#define TREECONTEXTMENU_HPP

#include <QObject>
#include <QContextMenuEvent>
#include <QMenu>
#include <QTreeWidget>

// Include TreeItem header
#include "TreeItem.hpp"

// Forward declaration for TreeStructure
class TreeStructure;
class TreeItem;

class TreeContextMenu : public QObject {
	Q_OBJECT
public :
	explicit TreeContextMenu(TreeStructure* treeWidget, QObject* parent = nullptr);
private slots:
	void showContextMenu(const QPoint& pos);

private:
	TreeStructure* _treeStructure;
	TreeItem* _selectedItem;

	// Method to create context menu based on clicked item
	QMenu* createContextMenu(TreeItem* item);

	void buildGeometryMenu(QMenu*);
	void buildMeshMenu(QMenu*);
	void buildResultsMenu(QMenu*);
	void buildSolutionMenu(QMenu*);

	void buildDefaultSubItemMenu(QMenu*);

	static const int fontSize = 10;

	QList<QAction*> geometryActions;
	QList<QAction*> meshActions;
	QList<QAction*> solutionActions;
	QList<QAction*> resultsActions;
	QList<QAction*> defaultSubItemActions;

	// Actions shared for all subitems
	QAction* renameItemAction;
	QAction* deleteItemAction;

	// Geometry actions
	QAction* addImporSTEPItemAction;
	
	// Mesh actions
	QAction* genereateMeshAction;
	QAction* addSizingAction;

	// Solution actions

	// Results actions
};

#endif