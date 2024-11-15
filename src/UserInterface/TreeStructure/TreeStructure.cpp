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

#include "TreeStructure.hpp"

//--------------------------------------------------------------------------------------
TreeStructure::TreeStructure(QWidget* parent)
	: QTreeWidget(parent),
	  _treeItemFactory(new TreeItemFactory(this)),
	  _rootItems({
          {ItemTypes::Root::Geometry, _treeItemFactory->createRootItem(ItemTypes::Root::Geometry)},
          {ItemTypes::Root::Mesh, 	  _treeItemFactory->createRootItem(ItemTypes::Root::Mesh)},
          {ItemTypes::Root::Solution, _treeItemFactory->createRootItem(ItemTypes::Root::Solution)},
          {ItemTypes::Root::Results,  _treeItemFactory->createRootItem(ItemTypes::Root::Results)}
      })
{
	QHeaderView* header = this->header();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
	header->setSectionResizeMode(QHeaderView::Interactive);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	_contextMenu = new TreeContextMenu(this);

	this->setColumnWidth(static_cast<int>(Column::Label), 10);
	this->setColumnWidth(static_cast<int>(Column::Visible), 10);
	this->setColumnWidth(static_cast<int>(Column::Actor), 10);
	this->header()->setVisible(true);  // Ensure headers are visible
	this->header()->setSectionResizeMode(QHeaderView::Stretch);

	
	std::for_each(_rootItems.begin(), _rootItems.end(), [this](auto item){this->addTopLevelItem(item.second);});
}

//--------------------------------------------------------------------------------------
TreeStructure::~TreeStructure() {
	#ifdef _WIN32
	std::string xmlPath = "test.xml";
	#endif
	#ifdef linux
	std::string xPath = "/mnt/Data/meshGenerator/MeshGeneratingTool/test.xml";
	#endif

	delete _treeItemFactory;
}

//--------------------------------------------------------------------------------------
void TreeStructure::addSubItem(TreeItem* aParenItem, const ItemTypes::Sub& aSubType){
	_treeItemFactory->createSubItem(aParenItem, aSubType);
}

TreeItem* TreeStructure::getRootItem(const ItemTypes::Root& aRootType){
	TreeItem* it = _rootItems.at(aRootType);
	return _rootItems.at(aRootType);
}

QList<TreeItem*> TreeStructure::getSubItems(const ItemTypes::Sub& aSubType){
    auto it = _subItems.find(aSubType);
    if (it != _subItems.end()) {
        return it->second;
    } 
    return {};
}

void TreeStructure::renameItem(QTreeWidgetItem* item){
	this->editItem(item, 0);
}

void TreeStructure::removeSubItem(QTreeWidgetItem* item){
	delete item;
}