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
	: QTreeWidget(parent) {
	QHeaderView* header = this->header();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
	header->setSectionResizeMode(QHeaderView::Interactive);

	this->docObjectModel = new QDomDocument();

	this->buildBaseObjectsRepresentation();

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	this->contextMenu = new TreeContextMenu(this);

    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
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
	for (auto& treeItem : domElements.keys()) {
		QTreeWidgetItem* item = treeItem;
		QSharedPointer<QDomElement> element = domElements.value(treeItem);

		int role = Role.value(element->firstChildElement("Properties").tagName());

		QVariant variantModel = item->data(0, role);
		QSharedPointer<PropertiesModel> model
			= variantModel.value<QSharedPointer<PropertiesModel>>();

		if (!model.isNull()) {
			model->deleteLater();
		}

		delete item;
	}
	delete this->docObjectModel;
	delete this->contextMenu;
}

//--------------------------------------------------------------------------------------
void TreeStructure::buildBaseObjectsRepresentation() {
	QString defaultPropsPath = AppDefaults::getInstance().getTemplatesPath();
	rapidjson::Document doc = this->readJsonTemplateFile(defaultPropsPath);

	QDomElement root = this->docObjectModel->createElement(
		AppDefaults::getInstance().getAppName());
	root.setAttribute(
		"version", AppDefaults::getInstance().getAppProjFileVersion());

	this->docObjectModel->appendChild(root);

	for (const auto& rootName : TreeRoots) {
		QString xmlTag = rootName;
		if (xmlTag.contains(" ")) {
			xmlTag.remove(" ");
		}
		
		QSharedPointer<QDomElement> rootElement(new QDomElement(this->docObjectModel->createElement(xmlTag)));

		PropertiesList propertiesList = this->parseDefaultProperties(doc, xmlTag);
		this->addProperties(rootElement, propertiesList);

		QTreeWidgetItem* rootItem = this->createTreeWidgetItem(rootElement);
		rootItem->setText(static_cast<int>(Column::Label), rootName);

		// This will parse only element with "Properties" tage name
		auto propsChildElem = rootElement->childNodes().at(0).toElement();
		QSharedPointer<QDomElement> propsChildElemPtr(&propsChildElem);
		this->addPropertiesModel(propsChildElemPtr, rootItem);
		root.appendChild(*rootElement);
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
QTreeWidgetItem* TreeStructure::createTreeWidgetItem(QSharedPointer<QDomElement> element, QTreeWidgetItem* parent) {
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

void TreeStructure::addMeshSizing() {
	QString defaultPropsPath = AppDefaults::getInstance().getDefaultPropertiesPath();
    rapidjson::Document doc = this->readJsonTemplateFile(defaultPropsPath);
	QString meshSizingTag = XMLTags.value(XMLTag::MeshSizing);
    PropertiesList propList = parseDefaultProperties(doc, meshSizingTag);
    
    QTreeWidgetItem* parentItem = getRootTreeWidgetItem(TreeRoot::Mesh);

	QSharedPointer<QDomElement> meshElement = domElements.value(parentItem);
	QString baseName = "Mesh sizing";
	QString meshElementName = getUniqueElementNameForTag(TreeRoot::Mesh, XMLTag::MeshSizing, baseName);

    // Create a new QDomElement dynamically
    QSharedPointer<QDomElement> meshSizingElementPtr(new QDomElement(this->docObjectModel->createElement(meshSizingTag)));
    meshSizingElementPtr->setAttribute("name", meshElementName);

    addProperties(meshSizingElementPtr, propList);
    
	QTreeWidgetItem* meshSizingItem = this->createTreeWidgetItem(meshSizingElementPtr, parentItem);
    meshSizingItem->setText(static_cast<int>(Column::Label), meshSizingElementPtr->attribute("name"));
	meshSizingItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);

    domElements.value(parentItem)->appendChild(*meshSizingElementPtr);

	auto propsChildElem = meshSizingElementPtr->childNodes().at(0).toElement();
    QSharedPointer<QDomElement> propsChildElemPtr(new QDomElement(propsChildElem));
	this->addPropertiesModel(propsChildElemPtr, meshSizingItem);

    connect(this, &QTreeWidget::itemChanged, this, &TreeStructure::onItemChanged);

	std::cout << "Name attribute in method: " << meshSizingElementPtr->attribute("name").toStdString() << std::endl;

    // Add the new mesh sizing element to the map
}
QString TreeStructure::getUniqueElementNameForTag(TreeRoot root, XMLTag tag, const QString& baseName){
	QTreeWidgetItem* rootItem = getRootTreeWidgetItem(root);
	QDomNodeList rootElementChildNodes = domElements.value(rootItem)->childNodes();
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
void TreeStructure::addNode(const QString& parentLabel, const QString& nodeName) {
	QList<QTreeWidgetItem*> qlist
		= this->findTreeWidgetItems(parentLabel, Qt::MatchExactly);

	QTreeWidgetItem* parentItem = nullptr;
	if (!qlist.isEmpty()) {
		parentItem = qlist.first();
	}

	QDomElement element = this->docObjectModel->createElement("group");
	element.setAttribute("label", nodeName);

	if (domElements.contains(parentItem)) {
		QSharedPointer<QDomElement> node = domElements.value(parentItem);
		if (node) {
			node->appendChild(element);
		}
	}
	QSharedPointer<QDomElement> elementPtr(&element);
	auto item = this->createTreeWidgetItem(elementPtr, parentItem);
	item->setText(static_cast<int>(Column::Label), nodeName);
}

//--------------------------------------------------------------------------------------
void TreeStructure::addPropertiesModel(QSharedPointer<QDomElement> element, QTreeWidgetItem* item) {
	const int role = Role.value(element->tagName());
	QSharedPointer<PropertiesModel> model(new PropertiesModel(element, this));
	QVariant variantModel = QVariant::fromValue(model);
	// ToDo: Model data changed detection
	item->setData(0, role, variantModel);
}

//--------------------------------------------------------------------------------------
void TreeStructure::addProperties(QSharedPointer<QDomElement> parentElement, PropertiesList propertiesList) {
	QDomElement propsElement = this->docObjectModel->createElement("Properties");

	for (const QMap<QString, QString>& propertyMap : propertiesList) {
		QDomElement elem = this->docObjectModel->createElement("property");

		// Iterate through QMap to set attributes
		for (const auto& key : propertyMap.keys()) {
			if (key != "value") {
				elem.setAttribute(key, propertyMap.value(key));
			}
		}

		// Set the value of node
		QDomText textNode = this->docObjectModel->createTextNode(propertyMap.value("value"));
		elem.appendChild(textNode);

		propsElement.appendChild(elem);
	}

	parentElement->appendChild(propsElement);
}

//--------------------------------------------------------------------------------------
PropertiesList TreeStructure::parseDefaultProperties(const rapidjson::Document& document, QString prop) {
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

	// Parse the JSON string
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

void TreeStructure::treeWidgetItemRenamed(QTreeWidgetItem* renamedItem, QString newName) {
    QSharedPointer<QDomElement> elementToRename = domElements.value(renamedItem, nullptr);
    
    if (elementToRename) {
        std::cout << "Element valid, name: " << elementToRename->attribute("name").toStdString() << std::endl;
        elementToRename->setAttribute("name", newName);
    } else {
        std::cout << "Element not found for the given QTreeWidgetItem" << std::endl;
    }
}

void TreeStructure::onItemChanged(QTreeWidgetItem* item, int column) {
    if (column == 0) { // Assuming we are renaming the first column
		QString newName = item->text(column);
        qDebug() << "Item changed, new name: " << newName;
        treeWidgetItemRenamed(item, newName);
    }
}
