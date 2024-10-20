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
#ifndef DOCUMENTHANDLER_HPP
#define DOCUMENTHANDLER_HPP

#include "TreeItemTypes.hpp"
#include "JsonParser.hpp"
#include "AppDefaults.h"

#include <QMap>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class DocumentHandler {
public:
    // Delete copy constructor and assignment operator
    DocumentHandler(const DocumentHandler&) = delete;
    DocumentHandler& operator=(const DocumentHandler&) = delete;

    // Accessor for the singleton instance
    static DocumentHandler& getInstance() {
        static DocumentHandler instance; // Guaranteed to be destroyed
        return instance;                  // Instantiated on first use
    }

    void writeDocToXML(const std::string& savePath);
    QDomElement createRootElement(const ItemTypes::Root& rootType);
    QDomElement createSubElement(const ItemTypes::Sub& rootType, QDomElement& parentElement);
    static QDomElement getPropertiesElement(const QDomElement& element);
    static void setNameProperty(const QDomElement& aElement, const QString& aName);

    QList<QDomElement> getElementsByType(const ItemTypes::Sub& subType);
    QDomElement getRootElement(const ItemTypes::Root& rootTag);
    QString getPropertyValue(const QDomElement& element, const QString& propName);

private:
    // Private constructor to prevent instantiation
    DocumentHandler();
    ~DocumentHandler();

    QDomDocument _domDocument;
    QDomElement _appRootElement;

    void addPropertiesToElement(QDomElement& element, const JsonParser::PropertiesMap& properties);
};

#endif // DOCUMENTHANDLER_HPP