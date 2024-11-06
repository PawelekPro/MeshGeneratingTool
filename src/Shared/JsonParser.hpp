#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#include "document.h"
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <vtkLogger.h>

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

class JsonParser {
    public:
        struct Property {
            Property() : name("Invalid") {};
            Property(const QString &aName) : name(aName) {};

            std::map<QString, QVariant> attributes;

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
        static Property mapToProperty(const QString &aPropName, const rapidjson::Value &aJsonValue);
};

#endif