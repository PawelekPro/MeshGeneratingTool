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

#include "TreeContextMenu.h"

TreeContextMenu::TreeContextMenu(TreeStructure* treeWidget, QObject* parent)
	: QObject(parent)
	, _treeWidget(treeWidget) {

	connect(
		this->_treeWidget, &TreeStructure::customContextMenuRequested,
		this, &TreeContextMenu::showContextMenu);
}

void TreeContextMenu::showContextMenu(const QPoint& pos) {
	QTreeWidgetItem* item = this->_treeWidget->itemAt(pos);
	if (!item)
		return;

	// Create a context menu based on the clicked item
	QMenu* contextMenu = createContextMenu(item);

	// Execute the context menu at the given position
	contextMenu->exec(this->_treeWidget->mapToGlobal(pos));

	// Cleanup
	contextMenu->deleteLater();
}

QMenu* TreeContextMenu::createContextMenu(QTreeWidgetItem* item) {
	QMenu* contextMenu = new QMenu(this->_treeWidget);

	QFont font;
	font.setPointSize(this->fontSize);
	contextMenu->setFont(font);
	int column = static_cast<int>(TreeStructure::Column::Label);

	if (item->text(column) == "Geometry Imports") {
		this->buildGeometryImportsMenu(contextMenu);

	} else if (item->text(column) == "Mesh") {
		this->buildMeshMenu(contextMenu);

	} else if (item->text(column) == "Geometry Model") {
		this->buildGeometryModelMenu(contextMenu);

	} else {
		this->buildCoordinateSystemMenu(contextMenu, item);
	}

	return contextMenu;
}

void TreeContextMenu::buildGeometryImportsMenu(QMenu* menu) {
	menu->addAction("Import STEP");
}

void TreeContextMenu::buildGeometryModelMenu(QMenu* menu) {
	menu->addAction("New model");
}

void TreeContextMenu::buildCoordinateSystemMenu(QMenu* menu, QTreeWidgetItem* item) {

	renameAction = menu->addAction("Rename");
    connect(renameAction, &QAction::triggered, [this, item]() {_treeWidget->renameItem(item); });

	removeItemAction = menu->addAction("Remove");
	connect(removeItemAction, &QAction::triggered, [this, item](){_treeWidget->removeSubItem(item);});
}

void TreeContextMenu::buildMeshMenu(QMenu* menu) {
	addSizingAction = menu->addAction("Add sizing");
	connect(addSizingAction, &QAction::triggered, [this](){_treeWidget->addMeshSizing();});
}