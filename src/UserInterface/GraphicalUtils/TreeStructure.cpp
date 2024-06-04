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
	: QTreeWidget(parent),
	  eventHandler(new TreeWidgetEventHandler) {
	QHeaderView* header = this->header();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
	header->setSectionResizeMode(QHeaderView::Interactive);

	this->addRootItems();

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	this->contextMenu = new TreeContextMenu(this);

    connect(this, &QTreeWidget::itemChanged, this, &TreeStructure::onItemChanged);
	}

//--------------------------------------------------------------------------------------
TreeStructure::~TreeStructure() {
	#ifdef _WIN32
	std::string xmlPath = "test.xml";
	#endif
	#ifdef linux
	std::string xPath = "/mnt/Data/meshGenerator/MeshGeneratingTool/test.xml";
	#endif
	this->writeDataToXML(xmlPath);

	// error causing part of destructor
	for (auto& treeItem : domElements.keys()) {
		QTreeWidgetItem* item = treeItem;
		QDomElement element = domElements.value(treeItem);
		int role = Role.value(element.firstChildElement("Properties").tagName());
		QVariant variantModel = item->data(0, role);
		QSharedPointer<PropertiesModel> model
			= variantModel.value<QSharedPointer<PropertiesModel>>();
		if (!model.isNull()) {
			model->deleteLater();
		}
	}
	domElements.clear();
	delete eventHandler;
}

//--------------------------------------------------------------------------------------
void TreeStructure::addRootItems() {
	QString rootPropertiesPath = AppDefaults::getInstance().getTemplatesPath();
	rapidjson::Document doc = this->readJsonTemplateFile(rootPropertiesPath);

	QDomElement docRootElement = this->docObjectModel.createElement(
		AppDefaults::getInstance().getAppName());
	docRootElement.setAttribute(
		"version", AppDefaults::getInstance().getAppProjFileVersion());

	this->docObjectModel.appendChild(docRootElement);

	for (const auto& rootName : TreeRoots) {
		QString xmlTag = rootName;
		if (xmlTag.contains(" ")) {
			xmlTag.remove(" ");
		}
		
		QDomElement rootElement = this->docObjectModel.createElement(xmlTag);

		PropertiesList propertiesList = this->parseJsonDocument(doc, xmlTag);
		this->addProperties(rootElement, propertiesList);

		QTreeWidgetItem* rootItem = this->createTreeWidgetItem(rootElement);
		rootItem->setText(static_cast<int>(Column::Label), rootName);

		// This will parse only element with "Properties" tage name
		auto propsChildElem = rootElement.childNodes().at(0).toElement();
		this->addPropertiesModel(propsChildElem, rootItem);
		docRootElement.appendChild(rootElement);
	}
}

//--------------------------------------------------------------------------------------
QList<QTreeWidgetItem*> TreeStructure::findTreeWidgetItems(
	const QString& qString, Qt::MatchFlags flags) {
	return this->findItems(qString, flags);
}

QTreeWidgetItem* TreeStructure::getRootTreeWidgetItem(TreeRoot root){
	
	QString rootName = TreeRoots.value(root);
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
	domElements[item] = element;
	return item;
}

QString TreeStructure::getUniqueElementNameForTag(QTreeWidgetItem* parentItem, XMLTag tag, const QString& baseName){
	QDomNodeList rootElementChildNodes = domElements.value(parentItem).childNodes();
	QSet<QString> rootElementChildNames;
	for(int i = 0; i < rootElementChildNodes.size(); ++i){
		QDomElement child = rootElementChildNodes.at(i).toElement();
        if (child.tagName() == XMLTags.value(tag)) {
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


//--------------------------------------------------------------------------------------
void TreeStructure::addPropertiesModel(const QDomElement& element, QTreeWidgetItem* item) {
	const int role = Role.value(element.tagName());
	QSharedPointer<PropertiesModel> model(new PropertiesModel(element, eventHandler, this));
	QVariant variantModel = QVariant::fromValue(model);
	// ToDo: Model data changed detection
	item->setData(0, role, variantModel);
}

//--------------------------------------------------------------------------------------
void TreeStructure::addProperties(QDomElement& parentElement, PropertiesList propertiesList) {
	QDomElement propsElement = this->docObjectModel.createElement("Properties");

	for (const QMap<QString, QString>& propertyMap : propertiesList) {
		QDomElement elem = this->docObjectModel.createElement("property");

		// Iterate through QMap to set attributes
		for (const auto& key : propertyMap.keys()) {
			if (key != "value") {
				elem.setAttribute(key, propertyMap.value(key));
			}
		}

		// Set the value of node
		QDomText textNode = this->docObjectModel.createTextNode(propertyMap.value("value"));
		elem.appendChild(textNode);

		propsElement.appendChild(elem);
	}
	parentElement.appendChild(propsElement);
}

//--------------------------------------------------------------------------------------
PropertiesList TreeStructure::parseJsonDocument(const rapidjson::Document& document, QString prop) {
	// Initialize properties container
	PropertiesList propertiesList;
	std::string propName = prop.toStdString();
	if (document.HasMember(propName.c_str()) && document[propName.c_str()].IsObject()) {
		const rapidjson::Value& section = document[propName.c_str()];

		if (section.HasMember("Properties")) {
			const rapidjson::Value& propertiesArray = section["Properties"];

			// Iterate through the elements of the "Properties" array
			for (rapidjson::SizeType i = 0; i < propertiesArray.Size(); ++i) {
				const rapidjson::Value& property = propertiesArray[i];

				if (property.IsObject()) {
					// Each QMap instance represents one property
					QMap<QString, QString> propertyMap;

					for (rapidjson::Value::ConstMemberIterator itr
						 = property.MemberBegin();
						 itr != property.MemberEnd(); ++itr) {
						// Get the key and value
						const char* key = itr->name.GetString();
						const rapidjson::Value& value = itr->value;

						// Insert key-value pair into the QMap
						propertyMap[QString::fromUtf8(key)]
							= QString::fromUtf8(value.GetString());
					}
					// Add the QMap to the QList
					propertiesList.append(propertyMap);
				}
			}
		}
	}
	return propertiesList;
}

//--------------------------------------------------------------------------------------
rapidjson::Document TreeStructure::readJsonTemplateFile(const QString& jsonTemplatePath) {
	QFile jsonFile(jsonTemplatePath);

	if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		vtkLogF(ERROR, "Failed to open ProjectSetup.json file.");
	}

	QByteArray jsonData = jsonFile.readAll();
	jsonFile.close();

	rapidjson::Document document;
	document.Parse(jsonData.constData());

	if (document.HasParseError()) {
		vtkLogF(ERROR, "Error parsing JSON file.");
	}

	return document;
}

//--------------------------------------------------------------------------------------
void TreeStructure::writeDataToXML(const std::string path) {
	QFile file(QString::fromStdString(path));

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Failed to open file for writing.";
		return;
	}

	QTextStream out(&file);
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	// Indentation of 4 spaces
	this->docObjectModel.save(out, 4);

	file.close();
}



//--------------------------------------------------------------------------------------
void TreeStructure::treeWidgetItemRenamed(QTreeWidgetItem* renamedItem, QString newName) {
    QDomElement elementToRename = domElements.value(renamedItem);
	elementToRename.setAttribute("name", newName);
}

void TreeStructure::onItemChanged(QTreeWidgetItem* item, int column) {
    if (column == 0) {
		QString newName = item->text(column);
        treeWidgetItemRenamed(item, newName);
    }
}
void TreeStructure::renameItem(QTreeWidgetItem* item){
	this->editItem(item, 0);
}
void TreeStructure::removeSubItem(QTreeWidgetItem* item){
	QDomElement element = domElements.value(item);
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
    domElements.remove(item);
    delete item;
}

//--------------------------------------------------------------------------------------
void TreeStructure::addSubItem(QTreeWidgetItem* parentItem,  XMLTag xmlTag, const QString& nodeBaseName,
	Qt::ItemFlags flags){
	if(!parentItem){
		qWarning("Can only add node to a parent.");
		return;
		}
	QDomElement parentElement = domElements.value(parentItem);
	QString newName = getUniqueElementNameForTag(parentItem, xmlTag, nodeBaseName);

   	QDomElement newElement = this->docObjectModel.createElement(XMLTags.value(xmlTag));
    newElement.setAttribute("name", newName);
	
	QString defaultPropsPath = AppDefaults::getInstance().getDefaultPropertiesPath();
    rapidjson::Document doc = this->readJsonTemplateFile(defaultPropsPath);
    PropertiesList propList = parseJsonDocument(doc, XMLTags.value(xmlTag));
	if(propList.isEmpty()){
		QString warning = "PropertiesList for node: " + newName + " is empty";
		qWarning() << warning;
	}
	addProperties(newElement, propList);
	QTreeWidgetItem* newItem = this->createTreeWidgetItem(newElement, parentItem);
    newItem->setText(static_cast<int>(Column::Label), newElement.attribute("name"));
	newItem->setFlags(flags);
    domElements.value(parentItem).appendChild(newElement);

	auto propsChildElem = newElement.childNodes().at(0).toElement();
	this->addPropertiesModel(propsChildElem, newItem);
}



void TreeStructure::loadGeometryFile(const QString fileName) {
	QTreeWidgetItem* parentItem = getRootTreeWidgetItem(TreeRoot::GeomModel);
	this->addSubItem(parentItem, XMLTag::GeometryImport, fileName);
	parentItem = getRootTreeWidgetItem(TreeRoot::GeomImport);
	this->addSubItem(parentItem, XMLTag::GeometryImport, fileName);
}

void TreeStructure::addMeshSizing() {
	QString baseName = "Mesh sizing";
	QTreeWidgetItem* meshParentItem = getRootTreeWidgetItem(TreeRoot::Mesh);
	addSubItem(meshParentItem, XMLTag::MeshSizing, baseName);
}


PropertiesList TreeStructure::getRootProperties(TreeRoot root) {
    QTreeWidgetItem* rootItem = getRootTreeWidgetItem(root);
    QDomElement rootElement = domElements.value(rootItem);
    QDomNodeList elementNodeList = rootElement.childNodes();
    PropertiesList propertiesList;

    for (int i = 0; i < elementNodeList.count(); ++i) {
        QDomNode node = elementNodeList.at(i);
        if (node.isElement()) {
            QDomElement element = node.toElement();
            if (element.tagName() == "Properties") {
                QMap<QString, QString> propertyMap;
                QDomNodeList properties = element.childNodes();
                for (int j = 0; j < properties.count(); ++j) {
                    QDomNode propertyNode = properties.at(j);
                    if (propertyNode.isElement()) {
                        QDomElement propertyElement = propertyNode.toElement();
                        QDomNamedNodeMap attributes = propertyElement.attributes();
                        for (int k = 0; k < attributes.count(); ++k) {
                            QDomNode attribute = attributes.item(k);
                            QString propertyName = attribute.nodeName();
                            QString propertyValue = attribute.nodeValue();
                            propertyMap.insert(propertyName, propertyValue);
                        }
						propertyMap.insert("value", propertyElement.text());
                		propertiesList.append(propertyMap);
                    }
                }
            }
        }
    }
    return propertiesList;
}


QList<PropertiesList> TreeStructure::getItemsProperties(TreeRoot root, XMLTag itemTag){
	QList<PropertiesList> list;
	return list;
};
