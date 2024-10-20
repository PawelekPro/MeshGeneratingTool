/*
 * Copyright (C) 2024 Krystian Fudali
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

#include "JsonParser.hpp"

rapidjson::Document JsonParser::initJsonDocument(const QString &filePath) {
    QFile jsonFile(filePath);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open JSON file:" << filePath;
        return rapidjson::Document(); 
    }
    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    std::string jsonString = jsonData.toStdString();

    rapidjson::Document document;
    document.Parse(jsonString.c_str());

    if (document.HasParseError()) {
        qWarning() << "Error parsing JSON file:" << filePath;
    }
    return document;
}
JsonParser::PropertiesMap JsonParser::getPropertiesMap(const rapidjson::Document &document, const QString &entryTag) {
    PropertiesMap propertiesMap;

    const char *entryName = entryTag.toStdString().c_str();

    if (!document.HasMember(entryName) || !document[entryName].IsObject()) {
        qWarning() << "Json document does not include entry:" << entryName;
        return propertiesMap;
    }

    const rapidjson::Value &entrySection = document[entryName];

    // Check if the entry has a "Properties" object
    if (!entrySection.HasMember("Properties") || !entrySection["Properties"].IsObject()) {
        qWarning() << "Entry does not have a valid Properties object";
        return propertiesMap;
    }

    const rapidjson::Value &propertiesObject = entrySection["Properties"];

    for (auto itr = propertiesObject.MemberBegin(); itr != propertiesObject.MemberEnd(); ++itr) {
        const QString propertyName = QString::fromStdString(itr->name.GetString());
        const rapidjson::Value &jsonPropertyValue = itr->value;

        Property property = mapToProperty(propertyName, jsonPropertyValue);
        propertiesMap[propertyName] = property;
    }
    return propertiesMap;
}

JsonParser::Property JsonParser::mapToProperty(const QString &aPropName, const rapidjson::Value &aJsonValue) {
    Property property(aPropName);

    for (auto itr = aJsonValue.MemberBegin(); itr != aJsonValue.MemberEnd(); ++itr) {
        const QString key = QString::fromStdString(itr->name.GetString());
        const rapidjson::Value &value = itr->value;

        if (value.IsInt()) {
            property.values[key] = value.GetInt();
        } else if (value.IsDouble()) {
            property.values[key] = value.GetDouble();
        } else if (value.IsString()) {
            property.values[key] = QString::fromStdString(value.GetString());
        } else if (value.IsBool()) {
            property.values[key] = value.GetBool();
        } else {
            qWarning() << "Unsupported value type for key:" << key;
        }
    }
    return property;
}