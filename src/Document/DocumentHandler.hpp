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

#include "DocItemTypes.hpp"
#include "JsonParser.hpp"
#include "AppInfo.hpp"

#include <QMap>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// TODO: Decouple DocumentHandler from AppInfo somehow. Think of better way to store paths

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

    void addTextNode(QDomElement& aElement, const QString& aValue);
    
    QDomElement createRootElement(const ItemTypes::Root& rootType);
    QDomElement createSubElement(const ItemTypes::Sub& rootType, QDomElement& parentElement);

    void removeElement(QDomElement& elementToRemove);

    QList<QDomElement> getElementsByType(const ItemTypes::Sub& subType) const;
    QDomElement getRootElement(const ItemTypes::Root& rootTag) const;

    void writeDocToXML(const std::string& savePath) const;

    static QDomElement getProperties(const QDomElement& aElement);
    static QDomElement getProperty(const QDomElement& aPropertiesElement, const QString& aPropName);

    static QString getPropertyValue(const QDomElement& aPropertyElement, const QString& aPropName);
    static QString getAttribute(const QDomElement& aPropertyElement, const QString& aAttributeName);
    static void setAttribute(QDomElement& aPropertyElement, const QString& aAttributeName, const QString& aNewValue);

    //TODO: add some docUtils in DocumentUtils and move such methods there
    static QString intsToString(const std::vector<int> aIntVec);

private:

    DocumentHandler();
    ~DocumentHandler() = default;

    QDomDocument _domDocument;
    QDomElement _appRootElement;

    void addPropertiesToElement(QDomElement& element, const JsonParser::Properties& properties);
    static QDomElement findSubElement(const QDomElement& aElement, const QString& aSearchedTag);
};

#endif