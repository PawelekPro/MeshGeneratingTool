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
#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "document.h"
#include <string>
#include <QDebug>
#include <QFile>
#include <vtkLogger.h>

/**
 * Defines a custom data structure for storing a list of properties.
 * Each property is represented as a map with key-value pairs.
 */
using PropertiesList = QList<QMap<QString, QString>>;

class JsonParser{
    public:
        /**
         * Create rapidjson::Document object based on the path to .json file
         * @param filePath path to .json file
         */
        rapidjson::Document initJsonDocumnet(const QString& filePath);
        
        /**
         * Parse "Properties" value in given JSON document for a specified entryTag in JSON for ex. Mesh
         * @param document rapidjson::Document to be parsed. Can be obtained using initJsonDocument method
         * @param entryName Name of the entry to be parsed.
         */
        PropertiesList parseEntryProperties(const rapidjson::Document& document, const QString& entryTag);

    private:
        /**
         * Helper method that creates a QMap of attributes from json value (value in json is defined by {})
         * @param jsonValue
         */
        QMap<QString, QString> createPropertyMap(const rapidjson::Value& jsonValue);
};

#endif