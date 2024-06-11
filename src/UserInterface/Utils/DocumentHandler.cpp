#include "DocumentHandler.h"

const QMap<DocumentHandler::EntryTag, QString> DocumentHandler::entryTags = {
    { DocumentHandler::EntryTag::MeshSizing, "MeshSizing" },
    { DocumentHandler::EntryTag::STEPImport, "STEPImport" },
};
const QMap<DocumentHandler::RootTag, QString> DocumentHandler::rootTags = {
    { DocumentHandler::RootTag::GeomImport, "GeometryImports" },
    { DocumentHandler::RootTag::GeomModel, "GeometryModel" },
    { DocumentHandler::RootTag::CSYS, "CoordinateSystem" },
    { DocumentHandler::RootTag::Mesh, "Mesh" }
};
DocumentHandler::DocumentHandler(){
    this->_appRootElement = this->_domDocument.createElement(
		AppDefaults::getInstance().getAppName());
	
    this->_appRootElement.setAttribute(
		"version", AppDefaults::getInstance().getAppProjFileVersion());

	this->_domDocument.appendChild(this->_appRootElement);
}
DocumentHandler::~DocumentHandler(){
    writeDocToXML("testPath.xml");
}


void DocumentHandler::writeDocToXML(const std::string& savePath){
    QFile file(QString::fromStdString(savePath));
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Failed to open file for writing.";
		return;
	}

	QTextStream out(&file);
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	// Indentation of 4 spaces
	this->_domDocument.save(out, 4);

	file.close();
}

QDomElement DocumentHandler::createRootElement( const RootTag& newElementTag){
    
    QString rootTagName = this->rootTags.value(newElementTag);
    QDomElement rootElement = this->_domDocument.createElement(rootTagName);

    JsonParser parser; 
    rapidjson::Document rootElementsDocument = parser.initJsonDocumnet(
        AppDefaults::getInstance().getTemplatesPath());
    PropertiesList propertiesList = parser.parseEntryProperties(rootElementsDocument, rootTagName);

    addPropertiesToElement(rootElement, propertiesList);
    this->_appRootElement.appendChild(rootElement);


    return rootElement;
}

QDomElement DocumentHandler::createSubElement(  const EntryTag& newElementTag,
                                                QDomElement& parentElement){

    const QString& newElementTagStr = entryTags.value(newElementTag); 
    
    JsonParser parser;
    rapidjson::Document subElementsDocument = parser.initJsonDocumnet(
        AppDefaults::getInstance().getDefaultPropertiesPath());
    PropertiesList propertiesList = parser.parseEntryProperties(subElementsDocument, newElementTagStr);

    if(!parentElement.isElement()){
        qWarning("Root element does not exist");
    }

    QDomElement element = this->_domDocument.createElement(newElementTagStr);
    addPropertiesToElement(element, propertiesList);
    
    parentElement.appendChild(element);
    return element;
}

void DocumentHandler::addPropertiesToElement(QDomElement& element, const PropertiesList& propertiesList){
    QDomElement propertiesElement = this->_domDocument.createElement("Properties");
    for(auto& propertiesMap : propertiesList){
        QDomElement propertyElement = this->_domDocument.createElement("property");
        for (auto it = propertiesMap.constBegin(); it != propertiesMap.constEnd(); ++it) {
            const QString& key = it.key();
            const QString& value = it.value();
            propertyElement.setAttribute(key, value);
        }
        propertiesElement.appendChild(propertyElement);
    }
    element.appendChild(propertiesElement);
}

QDomElement DocumentHandler::getElementsPropertiesElement(const QDomElement& element){
    QDomNodeList elementSubNodes = element.childNodes();
    QDomElement subElement;
    bool found = false;
    for (int i = 0; i < elementSubNodes.count(); ++i) {
        QDomNode node = elementSubNodes.at(i);
        if (!node.isElement()) {
            continue;
        }
        subElement = node.toElement();
        if(subElement.tagName() == "Properties"){
            found = true;
            break;
        }
    }
    if(!found)
        qWarning("Could not find Properties element, returning last sub element");
    return subElement;
}