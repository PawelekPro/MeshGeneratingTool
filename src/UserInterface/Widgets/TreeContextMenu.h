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

#ifndef TREECONTEXTMENU_H
#define TREECONTEXTMENU_H

// Forward declaration
class TreeStructure;

#include "TreeStructure.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QTreeWidget>

class TreeContextMenu : public QObject {
	Q_OBJECT
public:
	explicit TreeContextMenu(TreeStructure* treeWidget, QObject* parent = nullptr);
private slots:
	void showContextMenu(const QPoint& pos);

private:
	TreeStructure* _treeWidget;

	// Method to create context menu based on clicked item
	QMenu* createContextMenu(QTreeWidgetItem* item);

	void buildGeometryImportsMenu(QMenu*);
	void buildGeometryModelMenu(QMenu*);
	void buildCoordinateSystemMenu(QMenu* menu, QTreeWidgetItem* item);
	void buildMeshMenu(QMenu*);

	static const int fontSize = 10;

	QAction* renameAction;
	QAction* genereateMeshAction;
	QAction* addSizingAction;
};

#endif