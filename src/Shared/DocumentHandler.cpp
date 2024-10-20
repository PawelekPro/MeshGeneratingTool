#include "DocumentHandler.hpp"

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

QDomElement DocumentHandler::createRootElement( const ItemTypes::Root& rootType){
    
    QString rootTagLabel = ItemTypes::label(rootType);
    QDomElement rootElement = this->_domDocument.createElement(rootTagLabel);
    
    rapidjson::Document rootDocument = JsonParser::initJsonDocument(
        AppDefaults::getInstance().getRootItemsSetupPath());

    JsonParser::PropertiesMap properties = JsonParser::getPropertiesMap(rootDocument,
                                                                   rootTagLabel);

    addPropertiesToElement(rootElement, properties);
    this->_appRootElement.appendChild(rootElement);

    return rootElement;
}

QDomElement DocumentHandler::createSubElement(const ItemTypes::Sub& subType,
                                              QDomElement& parentElement){
    if(!parentElement.isElement()){
        qWarning("Root element does not exist");
    }
    const QString& subTagLabel = ItemTypes::label(subType);

    rapidjson::Document subDocument = JsonParser::initJsonDocument(
        AppDefaults::getInstance().getSubItemsSetupPath());

    JsonParser::PropertiesMap properties = JsonParser::getPropertiesMap(subDocument,
                                                                   subTagLabel);


    QDomElement element = this->_domDocument.createElement(subTagLabel);
    addPropertiesToElement(element, properties);
    parentElement.appendChild(element);
    
    return element;
}

void DocumentHandler::addPropertiesToElement(QDomElement& element, const JsonParser::PropertiesMap& properties){
    QDomElement propertiesElement = _domDocument.createElement("Properties");
    for(const auto & property : properties){
        QDomElement propertyElement =_domDocument.createElement(property.first);

        for(const auto & propPair : property.second.values){
            propertyElement.setAttribute(propPair.first, propPair.second.toString());
        }
        propertiesElement.appendChild(propertyElement);
    }

    element.appendChild(propertiesElement);
}


QDomElement DocumentHandler::getPropertiesElement(const QDomElement& element){
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

void DocumentHandler::setNameProperty(const QDomElement& aElement, const QString& aName){
    getPropertiesElement(aElement).setAttribute("name", aName);
}

QList<QDomElement> DocumentHandler::getElementsByType(const ItemTypes::Sub& subType){
    QList<QDomElement> elements;
    const QString subTypeLabel = ItemTypes::label(subType);
    QDomNodeList nodeList = this->_domDocument.elementsByTagName(subTypeLabel);
    for (int i = 0; i < nodeList.size(); ++i) {
            QDomNode node = nodeList.item(i);
            if (node.isElement()) {
                elements.append(node.toElement());
            }
        }
    return elements;
}


QDomElement DocumentHandler::getRootElement(const ItemTypes::Root& rootTag){
    QDomNodeList nodeList = this->_domDocument.elementsByTagName(ItemTypes::label(rootTag));
    QDomElement rootElement = nodeList.at(0).toElement();
    return rootElement;
}

QString DocumentHandler::getPropertyValue(const QDomElement& element, const QString& propName) {
    QDomElement propElement = getPropertiesElement(element);
    QDomNodeList propertiesList = propElement.childNodes();

    for (int i = 0; i < propertiesList.size(); i++) {
        QDomElement prop = propertiesList.at(i).toElement();
        if (prop.nodeName() == propName) {
            return prop.text();
        }
    }
    qWarning() << "Could not find property with name:" << propName;
    return QString();
}
