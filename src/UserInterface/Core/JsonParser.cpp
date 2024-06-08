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
};

PropertiesList JsonParser::parseEntryProperties(const rapidjson::Document& document, const QString& entryTag){
    
    PropertiesList propertiesList;

    std::string entryNameStd = entryTag.toStdString();
    const char* entryName = entryNameStd.c_str();

    if (!document.HasMember(entryName) || 
        !document[entryName].IsObject()) {
        qWarning("Json document does not include entry: %s", qPrintable(entryName));
        return propertiesList;
    }

    const rapidjson::Value& entrySection = document[entryName];

    if (!entrySection.HasMember("Properties") || !entrySection["Properties"].IsArray()) {
        qWarning("Entry does not have a valid Properties array");
        return propertiesList;
    }
    const rapidjson::GenericArray<true, rapidjson::Value> propertiesArray = entrySection["Properties"].GetArray();

    for(auto& property : propertiesArray){
        if(!property.IsObject()){
            qWarning("Property is not an object!");
            continue;
        }
        QMap<QString, QString> propertyMap = createPropertyMap(property);
        propertiesList.append(propertyMap);
    };
    return propertiesList;
};

QMap<QString, QString> JsonParser::createPropertyMap(const rapidjson::Value& jsonValue){
    QMap<QString, QString> propertyMap;

    for(auto itr = jsonValue.MemberBegin();
            itr != jsonValue.MemberEnd();
            itr++){

        const char* nameChar = itr->name.GetString();
        const rapidjson::Value& jsonValue = itr->value;

        QString name = QString::fromUtf8(nameChar);
        QString value = QString::fromUtf8(jsonValue.GetString());
        propertyMap.insert(name, value);
    }
    return propertyMap;
};

