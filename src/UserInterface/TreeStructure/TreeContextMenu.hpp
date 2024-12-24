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

#ifndef TREECONTEXTMENU_HPP
#define TREECONTEXTMENU_HPP

#include <QObject>
#include <QContextMenuEvent>
#include <QMenu>
#include <QTreeWidget>

class TreeStructure;
class TreeItem;

class TreeContextMenu : public QObject {
	Q_OBJECT
public :
	explicit TreeContextMenu(TreeStructure* treeWidget, QObject* parent = nullptr);
private slots:
	void showContextMenu(const QPoint& pos);

private:


	/**
	 * @brief Method that creates appropriate contextMenu based on the ItemType of TreeItem*
	 * 
	 * @param aItem Pointer to aTreeItem whose type defines the type of menu to be created
	 * @return Pointer to created context menu QMenu*.
	 */
	QMenu* createContextMenu(TreeItem* aItem);

	void buildGeometryMenu(QMenu*);
	void buildMeshMenu(QMenu*);
	void buildResultsMenu(QMenu*);
	void buildSolutionMenu(QMenu*);
	void buildDefaultSubItemMenu(QMenu*);

	static const int fontSize = 10;

	TreeStructure* _treeStructure;
	TreeItem* _selectedItem;

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