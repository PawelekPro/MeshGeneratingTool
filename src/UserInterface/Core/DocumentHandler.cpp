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



void DocumentHandler::initializeDocument(){

    JsonParser parser;
    rapidjson::Document jsonDocument = parser.initJsonDocumnet(
        AppDefaults::getInstance().getTemplatesPath());
    
    QDomElement docRootElement = this->_domDocument.createElement(
		AppDefaults::getInstance().getAppName());
	docRootElement.setAttribute(
		"version", AppDefaults::getInstance().getAppProjFileVersion());

	this->_domDocument.appendChild(docRootElement);

    for(auto& rootTag : this->rootTags.keys()){
        addRootElement(jsonDocument, rootTag);
    }
}


void DocumentHandler::addRootElement(const rapidjson::Document& document, RootTag& rootTag){
    
    QString rootTagName = this->rootTags.value(rootTag);
    QDomElement rootElement = this->_domDocument.createElement(rootTagName);

    JsonParser parser;
    PropertiesList propertiesList = parser.parseEntryProperties(document, rootTagName);

    addPropertiesToElement(rootElement, propertiesList);

    this->_domDocument.appendChild(rootElement);
}

void DocumentHandler::addPropertiesToElement(QDomElement& element, const PropertiesList& propertiesList){
    QDomElement propertiesElement = this->_domDocument.createElement("Properties");
    for(auto& propertiesMap : propertiesList){
        for (auto it = propertiesMap.constBegin(); it != propertiesMap.constEnd(); ++it) {
            const QString& key = it.key();
            const QString& value = it.value();
            QDomElement propertyElement = this->_domDocument.createElement("property");
            propertyElement.setAttribute(key, value);
            propertiesElement.appendChild(propertyElement);
        }
    }
    element.appendChild(propertiesElement);
}