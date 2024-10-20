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

            std::map<QString, QVariant> values;

            QVariant &at(const QString &key) {
                return values.at(key);
            }

            const QVariant &at(const QString &key) const {
                return values.at(key);
            }

            QVariant &operator[](const QString &key) {
                return values[key];
            }

            bool contains(const QString &key) const {
                return values.find(key) != values.end();
            }

            void insert(const QString &key, const QVariant &value) {
                values[key] = value;
            }

            QString name;
        };

        using PropertiesMap = std::map<QString, Property>;

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
        static PropertiesMap getPropertiesMap(const rapidjson::Document &document, const QString &entryTag);

    private:
        /**
         * Helper method that creates a PropertiesMap of attributes from json value (value in json is defined by {})
         * @param jsonValue
         */
        static Property mapToProperty(const QString &aPropName, const rapidjson::Value &aJsonValue);
};

#endif