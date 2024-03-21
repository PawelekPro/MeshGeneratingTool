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

#include "TreeStructure.h"

//--------------------------------------------------------------------------------------
TreeStructure::TreeStructure(QWidget* parent)
	: QTreeWidget(parent)
	, appInfo() {
	QHeaderView* header = this->header();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
	header->setSectionResizeMode(QHeaderView::Interactive);

	this->docObjectModel = new QDomDocument();

	this->buildBaseObjectsRepresentation();
}

//--------------------------------------------------------------------------------------
TreeStructure::~TreeStructure() {
	delete this->docObjectModel;

	for (auto it = domElements.begin(); it != domElements.end(); ++it) {
		delete it.key();
		delete it.value();
	}
}

//--------------------------------------------------------------------------------------
void TreeStructure::buildBaseObjectsRepresentation() {

	QDomElement root = this->docObjectModel->createElement(this->appInfo.getAppName());
	root.setAttribute("version", this->appInfo.getAppProjFileVersion());
	this->docObjectModel->appendChild(root);

	for (auto it = TreeRoots.begin(); it != TreeRoots.end(); ++it) {
		QString rootName = it.value();

		QString xmlTag(rootName);
		if (xmlTag.contains(" ")) {
			xmlTag.remove(" ");
		}
		QDomElement* rootElement = new QDomElement(this->docObjectModel->createElement(xmlTag));

		QTreeWidgetItem* rootItem = this->createItem(rootElement);
		rootItem->setText(static_cast<int>(Column::Label), rootName);

		this->addPropertiesModel(rootElement, rootItem);
		root.appendChild(*rootElement);
	}
}

//--------------------------------------------------------------------------------------
QList<QTreeWidgetItem*> TreeStructure::findTreeWidgetItems(
	const QString& qString, Qt::MatchFlags flags) {

	return this->findItems(qString, flags);
}

//--------------------------------------------------------------------------------------
QTreeWidgetItem* TreeStructure::createItem(QDomElement* element, QTreeWidgetItem* parent) {
	QTreeWidgetItem* item = nullptr;

	if (parent) {
		item = new QTreeWidgetItem(parent);
	} else {
		item = new QTreeWidgetItem(this);
	}
	domElements[item] = element;
	return item;
}

//--------------------------------------------------------------------------------------
void TreeStructure::loadGeometryFile(const QString fileName) {
	this->addNode(TreeRoots.value(TreeRoot::GeomImport), fileName);
	this->addNode(TreeRoots.value(TreeRoot::GeomModel), fileName);
}

//--------------------------------------------------------------------------------------
void TreeStructure::addNode(const QString& parentLabel, const QString& fileName) {
	QList<QTreeWidgetItem*> qlist
		= this->findTreeWidgetItems(parentLabel, Qt::MatchExactly);

	QTreeWidgetItem* parentItem = nullptr;
	if (!qlist.isEmpty()) {
		parentItem = qlist.first();
	}

	QDomElement element = this->docObjectModel->createElement("group");
	element.setAttribute("label", fileName);

	if (domElements.contains(parentItem)) {
		QDomElement* node = domElements.value(parentItem);
		if (node) {
			node->appendChild(element);
		}
	}

	auto item = this->createItem(&element, parentItem);
	item->setText(static_cast<int>(Column::Label), fileName);
}

//--------------------------------------------------------------------------------------
void TreeStructure::addPropertiesModel(QDomElement* element, QTreeWidgetItem* item) {
	int role = Role.value(element->tagName());

	PropertiesModel model(element, this);
	QVariant variantModel = QVariant::fromValue(&model);

	// ToDo: Model data changed detection
	// item->setData(0, role, variantModel);
}

//--------------------------------------------------------------------------------------
void TreeStructure::writeDataToXML(const std::string path) {
	QFile file(QString::fromStdString(path));

	// Open the file in write mode
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Failed to open file for writing.";
		return;
	}

	// Create a QTextStream object to write to the file
	QTextStream out(&file);

	// Add XML header with xml version and encoding
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	// Indentation of 4 spaces
	this->docObjectModel->save(out, 4);

	// Close the file
	file.close();
}