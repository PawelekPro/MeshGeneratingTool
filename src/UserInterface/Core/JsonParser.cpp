#include "JsonParser.h"

rapidjson::Document JsonParser::initJsonDocumnet(const QString& filePath){
	QFile jsonFile(filePath);
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

QDomElement JsonParser::parseEntryProperties(const rapidjson::Document& document, const QString& entryTag){
    QDomElement element;    

    std::string entryNameStd = entryTag.toStdString();
    const char* entryName = entryNameStd.c_str();

    if (!document.HasMember(entryName) || 
        !document[entryName].IsObject()) {
        qWarning("Json document does not include entry: %s", qPrintable(entryName));
        return element;
    }

    const rapidjson::Value& entrySection = document[entryName];

    if (!entrySection.HasMember("Properties") || !entrySection["Properties"].IsArray()) {
        qWarning("Entry does not have a valid Properties array");
        return element;
    }
    const rapidjson::GenericArray<true, rapidjson::Value> propertiesArray = entrySection["Properties"].GetArray();

    for(auto& property : propertiesArray){
        if(!property.IsObject()){
            qWarning("Property is not an object!");
            continue;
        }
        QDomElement propertyElement = createElementFromValue(property, "property");
        element.appendChild(propertyElement);
    };
    return element;
}

QDomElement JsonParser::createElementFromValue(const rapidjson::Value& jsonValue, const QString& newElementTag){
    QDomElement element;
    element.setTagName(newElementTag);

    for(auto itr = jsonValue.MemberBegin();
            itr != jsonValue.MemberEnd();
            itr++){

        const char* nameChar = itr->name.GetString();
        const rapidjson::Value& jsonValue = itr->value;

        QString name = QString::fromUtf8(nameChar);
        QString value = QString::fromUtf8(jsonValue.GetString());

        element.setAttribute(name, value);
    }
    return element;
}
