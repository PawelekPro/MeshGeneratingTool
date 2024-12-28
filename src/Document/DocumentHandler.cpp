#include "DocumentHandler.hpp"

DocumentHandler::DocumentHandler(){
    this->_appRootElement = this->_domDocument.createElement(
		AppInfo::getInstance().getAppName());
	
    this->_appRootElement.setAttribute(
		"version", AppInfo::getInstance().getAppProjFileVersion());

	this->_domDocument.appendChild(this->_appRootElement);
}

void DocumentHandler::writeDocToXML(const std::string& savePath) const {
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
        AppInfo::getInstance().getRootItemsSetupPath());

    JsonParser::Properties properties = JsonParser::getProperties(rootDocument,
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
        AppInfo::getInstance().getSubItemsSetupPath());

    JsonParser::Properties properties = JsonParser::getProperties(subDocument,
                                                                  subTagLabel);

    QDomElement element = this->_domDocument.createElement(subTagLabel);
    addPropertiesToElement(element, properties);
    parentElement.appendChild(element);
    return element;
}

void DocumentHandler::removeElement(QDomElement& aElementToRemove){
    QDomNode parentNode = aElementToRemove.parentNode();
    if(parentNode.isElement()){
        QDomElement parentElement = parentNode.toElement();
        parentElement.removeChild(aElementToRemove);
        return;
    }
}

void DocumentHandler::addTextNode(QDomElement& aElement, const QString& aValue){
    QDomText valueNode = _domDocument.createTextNode(aValue);
    aElement.appendChild(valueNode);
}

void DocumentHandler::appendExistingElement(QDomElement& aParentElement, QDomElement& aChildElement) {
    if (aParentElement.isNull()) {
        qWarning("Cannot append to a null parent element!");
        return;
    }

    if (aChildElement.isNull()) {
        qWarning("Cannot append a null child element!");
        return;
    }
    aParentElement.appendChild(aChildElement);
}

void DocumentHandler::addPropertiesToElement(QDomElement& element, const JsonParser::Properties& properties){
    QDomElement propertiesElement = _domDocument.createElement("Properties");
    for(const auto & property : properties){
        QDomElement propertyElement =_domDocument.createElement("Property");
        if(property.second.hasValue){
            addTextNode(propertyElement, property.second.value.toString());
        }
        for(const auto & attribute : property.second.attributes){
            propertyElement.setAttribute(attribute.first, attribute.second.toString());
        }
        propertiesElement.appendChild(propertyElement);
    }
    element.appendChild(propertiesElement);
}

QList<QDomElement> DocumentHandler::getElementsByType(const ItemTypes::Sub& subType) const {
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

QDomElement DocumentHandler::getRootElement(const ItemTypes::Root& rootTag) const {
    QDomNodeList nodeList = this->_domDocument.elementsByTagName(ItemTypes::label(rootTag));
    QDomElement rootElement = nodeList.at(0).toElement();
    return rootElement;
}

QDomElement DocumentHandler::getProperties(const QDomElement& aElement) {
    QDomNodeList elementSubNodes = aElement.childNodes();
    for (int i = 0; i < elementSubNodes.count(); ++i) {
        QDomNode node = elementSubNodes.at(i);
        if (node.isElement()) {
            QDomElement subElement = node.toElement();
            if (subElement.tagName() == "Properties") {
                return subElement;
            }
        }
    }
    qWarning("Could not find 'Properties' element, returning an empty QDomElement.");
    return QDomElement();
}

QDomElement DocumentHandler::getProperty(const QDomElement& aPropertiesElement, const QString& aPropName) {
    QDomNodeList propertyNodes = aPropertiesElement.elementsByTagName("Property");
    for (int i = 0; i < propertyNodes.count(); ++i) {
        QDomElement propertyElement = propertyNodes.at(i).toElement();
        if (!propertyElement.isNull() && propertyElement.hasAttribute("name")) {
            QString propertyName = propertyElement.attribute("name");
            if (propertyName == aPropName) {
                return propertyElement;
            }
        }
    }
    qWarning() << "Could not find property: " << aPropName << " in Properties element - returning empty QDomElement";
    return QDomElement();
}

QString DocumentHandler::getPropertyValue(const QDomElement& aPropertiesElement, const QString& aPropName){
    QDomElement propElement = getProperty(aPropertiesElement, aPropName);
    return propElement.text();
};

QString DocumentHandler::getAttribute(const QDomElement& aProp, const QString& aAttributeName) {
    if(aProp.hasAttribute(aAttributeName)){
        return aProp.attribute(aAttributeName);
    }
    
    qWarning() << "Could not find attribute: " << aAttributeName << " in element " << aProp.tagName(); 
    return QString();
}

void DocumentHandler::setAttribute(QDomElement& aProp, const QString& aAttributeName, const QString& aNewValue) {
    if(aProp.hasAttribute(aAttributeName)){
        aProp.setAttribute(aAttributeName, aNewValue);
    }
    qWarning() << "Could not find attribute: " << aAttributeName;
}


QDomElement DocumentHandler::findSubElement(const QDomElement& aElement, const QString& aSearchedTag){
    QDomNodeList subNodes = aElement.childNodes();
    for (int i = 0; i < subNodes.count(); ++i){
        QDomNode node = subNodes.at(i);
        if (node.isElement()) {
            QDomElement subElement = node.toElement();
            if (subElement.tagName() == aSearchedTag) {
                return subElement;
            }
        }
    }
    qWarning() << "Could not find element with tag: " << aSearchedTag;
    return QDomElement();
}

QString DocumentHandler::intsToString(const std::vector<int>& aIntsVec){
    if (aIntsVec.empty()) {
        return QString();
    }
    std::vector<QString> stringVec(aIntsVec.size());
    std::transform(aIntsVec.begin(), aIntsVec.end(), stringVec.begin(),
                   [](int value) { return QString::number(value); });

    QString result = std::accumulate(stringVec.begin() + 1, stringVec.end(), stringVec[0],
                                     [](const QString& a, const QString& b) { return a + ", " + b; });

    return result;
}