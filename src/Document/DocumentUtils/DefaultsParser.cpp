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


#include "DefaultsParser.hpp"
#include "AppInfo.hpp"
#include "DocUtils.hpp"

#include <QFile>
#include <stdexcept>

namespace DefaultsParser {


    rapidjson::Document initJsonDocument(const QString& aJsonFilePath) {
        try {
            QFile file(aJsonFilePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw std::runtime_error("Failed to open JSON file: " + aJsonFilePath.toStdString());
            }

            QTextStream in(&file);
            QString fileContent = in.readAll();
            file.close();

            rapidjson::Document doc;
            if (doc.Parse(fileContent.toStdString().c_str()).HasParseError()) {
                throw std::runtime_error("Failed to parse JSON content from file: " + aJsonFilePath.toStdString());
            }

            return doc;
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return rapidjson::Document();
        }
    }

    QDomElement getItemProperties(QDomDocument& aDomDoc, const ItemTypes::Root& aItemType){
        rapidjson::Document aJsonDoc = initJsonDocument(AppInfo::rootItemsSetupPath);
        QString itemTypeLabel = ItemTypes::label(aItemType);
        QDomElement properties = getPropertiesFromJson(aDomDoc, aJsonDoc, itemTypeLabel);
        return properties;       
    }

    QDomElement getItemProperties(QDomDocument& aDomDoc, const ItemTypes::Sub& aItemType){
        rapidjson::Document aJsonDoc = initJsonDocument(AppInfo::subItemsSetupPath);
        QString itemTypeLabel = ItemTypes::label(aItemType);
        QDomElement properties = getPropertiesFromJson(aDomDoc, aJsonDoc, itemTypeLabel);        
        return properties;
    }

    QStringList getComboBoxList(const QString& aModelType){
        rapidjson::Document jsonDoc = initJsonDocument(AppInfo::comboBoxModelsPath);
        QStringList comboBoxList = getComboBoxListFromJson(jsonDoc, aModelType);
        return comboBoxList;
    }

    QStringList getComboBoxList(const ComboBoxTypes& aComboBoxType){
        rapidjson::Document jsonDoc = initJsonDocument(AppInfo::comboBoxModelsPath);
        QString comboBoxLabel = ComboBoxUtils::label(aComboBoxType);
        QStringList comboBoxList = getComboBoxListFromJson(jsonDoc, comboBoxLabel);
        return comboBoxList;
    }

    QStringList getComboBoxListFromJson(const rapidjson::Document& aJsonDoc, const QString& aComboBoxEntry){
        std::string comboBoxEntryString = aComboBoxEntry.toStdString();
        const char* comboBoxLabel = comboBoxEntryString.c_str();
        if (!aJsonDoc.HasMember(comboBoxLabel)
            || !(aJsonDoc)[comboBoxLabel].IsObject()) {
                qWarning() << "Json document does not include entry: " << aComboBoxEntry;
            return QStringList();
        }

        const rapidjson::Value& jsonValue = (aJsonDoc)[comboBoxLabel];

        std::vector<int> ids;
        std::vector<QString> m_list;

        for (const auto& pair : jsonValue.GetObject()) {
            const QString key = QString::fromStdString(pair.name.GetString());
            int id = key.toInt();
            ids.push_back(id);

            const QString label = QString::fromStdString(pair.value["label"].GetString());
            m_list.push_back(label);
        }

        // std::sort(ids.begin(), ids.end());

        QStringList qStringList;
        for (int id : ids) {
            qStringList << m_list[id];
        }
        return qStringList;
    }


    QDomElement getPropertiesFromJson(QDomDocument& aDomDoc, const rapidjson::Document& aJsonDoc, const QString& aJsonItemEntry){
        QDomElement properties = aDomDoc.createElement("Properties");
        std::string jsonEntryString = aJsonItemEntry.toStdString();
        const char *itemType = jsonEntryString.c_str();
        if (!aJsonDoc.HasMember(itemType) || !aJsonDoc[itemType].IsObject()) {
            qWarning() << "Json document does not include entry:" << aJsonItemEntry;
            return properties;
        }

        const rapidjson::Value &entrySection = aJsonDoc[itemType];

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
            QDomElement property = jsonValueToProperty(aDomDoc, jsonPropertyValue);
            if(!property.isNull()){
                properties.appendChild(property);
            }
        }
        return properties;
    }

    QDomElement jsonValueToProperty(QDomDocument& aDomDoc, const rapidjson::Value& aJsonValue) {    
    QDomElement property = aDomDoc.createElement("Property");

    for (auto itr = aJsonValue.MemberBegin(); itr != aJsonValue.MemberEnd(); ++itr) {
        const QString key = QString::fromStdString(itr->name.GetString());
        const rapidjson::Value& value = itr->value;
        QVariant valueVariant;

        if (value.IsInt()) {
            valueVariant = value.GetInt();
        } else if (value.IsDouble()) {
            valueVariant = value.GetDouble();
        } else if (value.IsString()) {
            QString qValueString = QString::fromStdString(value.GetString());
            valueVariant = QVariant(qValueString);
        } else if (value.IsBool()) {
            valueVariant = value.GetBool();
        } else {
            qWarning() << "Unsupported value type for key:" << key;
            continue;
        }

        if (key == "value") {
            Properties::setPropertyValue(property, valueVariant.toString());
        } else {
            Properties::setPropertyAttribute(property, key, valueVariant.toString());
        }
    }

    if (Properties::isProperty(property)) {
        return property;
    } else {
        qWarning() << "Could not translate json property to QDomElement";
        return QDomElement();
    }
}
}