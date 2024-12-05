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

#include "TreeContextMenu.hpp"
#include "TreeStructure.hpp"
#include "TreeItem.hpp"

TreeContextMenu::TreeContextMenu(TreeStructure* treeWidget, QObject* parent)
	: QObject(parent)
	, _treeStructure(treeWidget) {

	connect(
		this->_treeStructure, &TreeStructure::customContextMenuRequested,
		this, &TreeContextMenu::showContextMenu);
}

void TreeContextMenu::showContextMenu(const QPoint& pos) {
	TreeItem* item = dynamic_cast<TreeItem*>(this->_treeStructure->itemAt(pos));
	if (!item)
		return;
	_selectedItem = item;

	// Create a context menu based on the clicked item
	QMenu* contextMenu = createContextMenu(item);

	// Execute the context menu at the given position
	contextMenu->exec(this->_treeStructure->mapToGlobal(pos));

	// Cleanup
	contextMenu->deleteLater();
}

QMenu* TreeContextMenu::createContextMenu(TreeItem* item) {
	QMenu* contextMenu = new QMenu(this->_treeStructure);

	QFont font;
	font.setPointSize(this->fontSize);
	contextMenu->setFont(font);
	int column = static_cast<int>(TreeStructure::Column::Label);

	if(item->isRoot()){
		switch(item->rootType()){
			case ItemTypes::Root::Geometry:
				buildGeometryMenu(contextMenu);
				break;
			case ItemTypes::Root::Mesh:
				buildMeshMenu(contextMenu);
				break;
			case ItemTypes::Root::Solution:
				buildSolutionMenu(contextMenu);
				break;
			case ItemTypes::Root::Results:
				buildResultsMenu(contextMenu);
				break;
		}
	} else {
		buildDefaultSubItemMenu(contextMenu);
	}
	return contextMenu;
}

void TreeContextMenu::buildGeometryMenu(QMenu* aContextMenu){
	if(geometryActions.empty()){
		//TODO: think of moving the actions to another file
		
		ItemTypes::Sub importType{ItemTypes::Geometry::ImportSTEP};
		QString label = ItemTypes::label(importType);

		addImporSTEPItemAction = new QAction(label, _treeStructure);

		connect(addImporSTEPItemAction, &QAction::triggered, _treeStructure->geoHandler(), &GeometryActionsHandler::importSTEP);
	}
	aContextMenu->addActions(geometryActions);
}

void TreeContextMenu::buildMeshMenu(QMenu* aContextMenu){
	if(meshActions.empty()){
		genereateMeshAction = new QAction(_treeStructure);
		meshActions.append(genereateMeshAction);

		// connect(genereateMeshAction, &QAction::triggered, _treeStructure->meshHandler(),
		// 		&MeshActionHandler::meshSurface);

		ItemTypes::Sub sizingType{ItemTypes::Mesh::ElementSizing};
		QString label = ItemTypes::label(sizingType);

		addSizingAction = new QAction(label, _treeStructure);
		meshActions.append(addSizingAction);

		connect(addSizingAction, &QAction::triggered, _treeStructure,
			[this, sizingType]() { 
				_treeStructure->addSubItem(_selectedItem, sizingType);
			});
	}
	aContextMenu->addActions(meshActions);
}

void TreeContextMenu::buildSolutionMenu(QMenu* aContextMenu){
	
}

void TreeContextMenu::buildResultsMenu(QMenu* aContextMenu){
	
}

void TreeContextMenu::buildDefaultSubItemMenu(QMenu* aContextMenu){
	if(defaultSubItemActions.empty()){
		renameItemAction = new QAction(_treeStructure);
		defaultSubItemActions.append(renameItemAction);
		
		deleteItemAction = new QAction(_treeStructure);
		defaultSubItemActions.append(deleteItemAction);
	}
	aContextMenu->addActions(defaultSubItemActions);
}