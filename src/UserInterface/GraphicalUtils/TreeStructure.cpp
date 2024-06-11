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

#include "TreeStructure.h"

//--------------------------------------------------------------------------------------
TreeStructure::TreeStructure(QWidget* parent)
	: QTreeWidget(parent),
	  _eventHandler(new TreeWidgetEventHandler)
	{
	QHeaderView* header = this->header();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
	header->setSectionResizeMode(QHeaderView::Interactive);

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	this->_contextMenu = new TreeContextMenu(this);

    connect(this, &QTreeWidget::itemChanged, this, &TreeStructure::onItemRenamed);
	// _documentHandler->initializeDocument();
	}

void TreeStructure::initialize(DocumentHandler* documentHandler){
	this->_documentHandler = documentHandler;
	for(const auto& rootTag : DocumentHandler::rootTags.keys()){
		this->addRootItem(rootTag);
	}
}

//--------------------------------------------------------------------------------------
TreeStructure::~TreeStructure() {
	#ifdef _WIN32
	std::string xmlPath = "test.xml";
	#endif
	#ifdef linux
	std::string xmlPath = "/mnt/Data/meshGenerator/MeshGeneratingTool/test.xml";
	#endif

	// this->_documentHandler->writeDocToXML("testHandler.xml");

	for (auto& treeItem : _domElements.keys()) {
		QTreeWidgetItem* item = treeItem;
		QDomElement element = _domElements.value(treeItem);
		int role = Role.value(element.firstChildElement("Properties").tagName());
		QVariant variantModel = item->data(0, role);
		QSharedPointer<PropertiesModel> model
			= variantModel.value<QSharedPointer<PropertiesModel>>();
		if (!model.isNull()) {
			model->deleteLater();
		}
	}
	_domElements.clear();
}

//--------------------------------------------------------------------------------------

void TreeStructure::addRootItem(const DocumentHandler::RootTag& rootTag) {

	QDomElement rootElement = this->_documentHandler->createRootElement(rootTag);
	QTreeWidgetItem* rootItem = this->createTreeWidgetItem(rootElement);

	QString rootText = DocumentHandler::rootTags.value(rootTag);
	std::cout << "Root element tag name: " << rootElement.tagName().toStdString() << std::endl;
	std::cout << "Root element text: " << rootText.toStdString() << std::endl;
	rootItem->setText(static_cast<int>(Column::Visible), rootText);
	this->addPropertiesModel(rootElement, rootItem);
}

void TreeStructure::addSubItem(	QTreeWidgetItem* parentItem,
  								const DocumentHandler::EntryTag& entryTag,
								const QString& itemBaseName,
								Qt::ItemFlags flags){
	if(!parentItem){
		qWarning("Can only add sub item to existing parent item.");
		return;
		}
	QDomElement parentElement = _domElements.value(parentItem);
	QDomElement subElement = this->_documentHandler->createSubElement(entryTag, parentElement);

	QString subName = getUniqueElementNameForTag(parentItem, entryTag, itemBaseName);
	subElement.attribute("name") = subName;

	QTreeWidgetItem* newItem = this->createTreeWidgetItem(subElement, parentItem);
    newItem->setText(static_cast<int>(Column::Label), subName);
	newItem->setFlags(flags);

	QDomElement subElementProperties = this->_documentHandler->getElementsPropertiesElement(subElement);
	this->addPropertiesModel(subElementProperties, newItem);
}

//--------------------------------------------------------------------------------------

QTreeWidgetItem* TreeStructure::getRootTreeWidgetItem(const DocumentHandler::RootTag& rootTag){
	QString rootName = DocumentHandler::rootTags.value(rootTag);
	QList<QTreeWidgetItem*> itemList = this->findItems(rootName, Qt::MatchExactly); 
	if(itemList.isEmpty()){
		qErrnoWarning("Root TreeWidgetItem not initialized");
	}else if(itemList.size()!=1){
		qWarning("More than one elements with root name");
	}
	return itemList.first();
}

//--------------------------------------------------------------------------------------
QTreeWidgetItem* TreeStructure::createTreeWidgetItem(const QDomElement& element, QTreeWidgetItem* parent) {
	QTreeWidgetItem* item = nullptr;
	if (parent) {
		item = new QTreeWidgetItem(parent);
	} else {
		item = new QTreeWidgetItem(this);
	}
	_domElements[item] = element;
	return item;
}

QString TreeStructure::getUniqueElementNameForTag(QTreeWidgetItem* parentItem,
												const DocumentHandler::EntryTag& entryTag,
												const QString& baseName){
	
	QDomElement parentElement = _domElements.value(parentItem);
	QDomNodeList parentElementChildNodes = parentElement.childNodes();
	QSet<QString> rootElementChildNames;

	for(int i = 0; i < parentElementChildNodes.size(); ++i){
		QDomElement child = parentElementChildNodes.at(i).toElement();
        if (child.tagName() == DocumentHandler::entryTags.value(entryTag)) {
            QString name = child.attribute("name");
            if (!name.isEmpty()) {
                rootElementChildNames.insert(name);
            }
        }
	}
    int counter = 1;
    QString uniqueName;
    do {
        uniqueName = baseName + " " + QString::number(counter);
        counter++;
    } while (rootElementChildNames.contains(uniqueName));
	return uniqueName;
}
//--------------------------------------------------------------------------------------

void TreeStructure::addPropertiesModel(const QDomElement& element, QTreeWidgetItem* item) {
	const int role = Role.value(element.tagName());
	QSharedPointer<PropertiesModel> model(new PropertiesModel(element, _eventHandler, this));
	QVariant variantModel = QVariant::fromValue(model);
	// ToDo: Model data changed detection
	item->setData(0, role, variantModel);
}

//--------------------------------------------------------------------------------------

void TreeStructure::treeWidgetItemRenamed(QTreeWidgetItem* renamedItem, QString newName) {
    QDomElement elementToRename = _domElements.value(renamedItem);
	elementToRename.setAttribute("name", newName);
}

void TreeStructure::onItemRenamed(QTreeWidgetItem* item, int column) {
    if (column == 0) {
		QString newName = item->text(column);
        treeWidgetItemRenamed(item, newName);
    }
}
void TreeStructure::renameItem(QTreeWidgetItem* item){
	this->editItem(item, 0);
}

void TreeStructure::removeSubItem(QTreeWidgetItem* item){
	QDomElement element = _domElements.value(item);
	QString tag = element.tagName();
	//To do remove items from model based on the tagName. For ex. remove imported parts if import item is removed.
    if (!element.isNull()) {
        QDomNode parentNode = element.parentNode();
        if (!parentNode.isNull()) {
            parentNode.removeChild(element);
        }
    }
    int column = static_cast<int>(Column::Label);
    this->removeItemWidget(item, column);
    _domElements.remove(item);
    delete item;
}

//--------------------------------------------------------------------------------------

void TreeStructure::loadGeometryFile(const QString& fileName) {
	QTreeWidgetItem* parentItem = getRootTreeWidgetItem(DocumentHandler::RootTag::GeomModel);
	this->addSubItem(parentItem, DocumentHandler::EntryTag::STEPImport, fileName);
	parentItem = getRootTreeWidgetItem(DocumentHandler::RootTag::GeomImport);
	this->addSubItem(parentItem, DocumentHandler::EntryTag::STEPImport, fileName);
}

void TreeStructure::addMeshSizing() {
	QString baseName = "Mesh sizing";
	QTreeWidgetItem* meshParentItem = getRootTreeWidgetItem(DocumentHandler::RootTag::Mesh);
	addSubItem(meshParentItem, DocumentHandler::EntryTag::MeshSizing, baseName);
}