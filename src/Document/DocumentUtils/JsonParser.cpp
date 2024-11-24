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

JsonParser::Properties JsonParser::getProperties(const rapidjson::Document &aDocument, const QString &aEntryTag) {
    Properties properties;

    std::string entryNameStr = aEntryTag.toStdString();
    const char *entryName = entryNameStr.c_str();

    if (!aDocument.HasMember(entryName) || !aDocument[entryName].IsObject()) {
        qWarning() << "Json document does not include entry:" << entryName;
        return properties;
    }

    const rapidjson::Value &entrySection = aDocument[entryName];

    if (!entrySection.HasMember("Properties") || !entrySection["Properties"].IsArray()) {
        qWarning() << "Entry does not have a valid Properties array";
        return properties;
    }

    const rapidjson::Value &propertiesArray = entrySection["Properties"];

    for (auto itr = propertiesArray.Begin(); itr != propertiesArray.End(); ++itr) {
        if (!itr->IsObject()) {
            qWarning() << "Invalid property entry in Properties array";
            continue;
        }

        const rapidjson::Value &jsonPropertyValue = *itr;
        Property property = mapToProperty(jsonPropertyValue);
        if (property.name.isEmpty()) {
            qWarning() << "Property entry missing 'name' attribute";
            continue;
        }
        properties[property.name] = property;
    }

    return properties;
}
JsonParser::Property JsonParser::mapToProperty(const rapidjson::Value &aJsonValue) {

    Property property;
    for (auto itr = aJsonValue.MemberBegin(); itr != aJsonValue.MemberEnd(); ++itr) {
        const QString key = QString::fromStdString(itr->name.GetString());
        const rapidjson::Value &value = itr->value;
        QVariant valueVariant;
        if (value.IsInt()) {
            valueVariant = value.GetInt();
        } else if (value.IsDouble()) {
            valueVariant = value.GetDouble();
        } else if (value.IsString()) {
            valueVariant = QString::fromStdString(value.GetString());
        } else if (value.IsBool()) {
            valueVariant = value.GetBool();
        } else {
            qWarning() << "Unsupported value type for key:" << key;
        }
        if (key == "value"){
            property.value = valueVariant;
            property.hasValue = true;
        } else {
            property.attributes[key] = valueVariant;
        }
        if (key == "name"){
            property.name = valueVariant.toString();
        }
    }
    return property;
}