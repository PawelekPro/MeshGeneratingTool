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

#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#include "document.h"
#include <QString>
#include <QVariant>
#include <QFile>
#include <unordered_map>

class JsonParser {
    public:
        // TODO: Rework this, think of some architecture to parse properties. Maybe we should have them 
        //       hard-coded with some maps and enums? 
        struct Property {
            Property() : name("Invalid") {};
            Property(const QString &aName) : name(aName) {};

            std::unordered_map<QString, QVariant> attributes;

            QVariant &at(const QString &attributeKey) {
                return attributes.at(attributeKey);
            }

            const QVariant &at(const QString &attributeKey) const {
                return attributes.at(attributeKey);
            }

            QVariant &operator[](const QString &attributeKey) {
                return attributes[attributeKey];
            }

            bool contains(const QString &attributeKey) const {
                return attributes.find(attributeKey) != attributes.end();
            }

            void insert(const QString &attributeKey, const QVariant &attributeValue) {
                attributes[attributeKey] = attributeValue;
            }

            QString name;
            QVariant value;
            bool hasValue = false;
        };

        using Properties = std::map<QString, Property>;

        /**
         * Create rapidjson::Document object based on the path to .json file
         * @param filePath path to .json file
         */
        static rapidjson::Document initJsonDocument(const QString &filePath);

        /**
         * Parse "Properties" value in given JSON document for a specified entryTag in JSON for ex. Mesh
         * @param document rapidjson::Document to be parsed. Can be obtained using initJsonDocument method
         * @param entryName Name of the entry to be parsed.
         */
        static Properties getProperties(const rapidjson::Document &document, const QString &entryTag);

    private:
        /**
         * Helper method that creates a PropertiesMap of attributes from json value (value in json is defined by {})
         * @param jsonValue
         */
        static Property mapToProperty(const rapidjson::Value &aJsonValue);
};

#endif