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
    QDomElement properties = DefaultsParser::getItemProperties(_domDocument, rootType);
    rootElement.appendChild(properties);

    this->_appRootElement.appendChild(rootElement);
    return rootElement;
}

QDomElement DocumentHandler::createSubElement(const ItemTypes::Sub& subType,
                                              QDomElement& parentElement){
    if(!parentElement.isElement()){
        qWarning("Root element does not exist");
    }
    const QString& subTagLabel = ItemTypes::label(subType);
    QDomElement element = this->_domDocument.createElement(subTagLabel);
    QDomElement properties = DefaultsParser::getItemProperties(_domDocument, subType);
    element.appendChild(properties);
    
    parentElement.appendChild(element);
    return element;
}

QList<QDomElement> DocumentHandler::getSubElements(const ItemTypes::Sub& aSubType) const {
    QDomNodeList nodeList = _domDocument.elementsByTagName(ItemTypes::label(aSubType));
    QList<QDomElement> elementList;
    for(int i = 0; i < nodeList.size(); ++i){
        QDomNode node = nodeList.at(i);
        if(node.isElement()){
            elementList.append(node.toElement());
        }
    }
    return elementList;
}

QDomElement DocumentHandler::getRootElement(const ItemTypes::Root& rootTag) const {
    QDomNodeList nodeList = this->_domDocument.elementsByTagName(ItemTypes::label(rootTag));
    QDomElement rootElement = nodeList.at(0).toElement();
    return rootElement;
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
