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
//       We would like this to be completely independet module for easier testing.

/**
 * Class for storing and handling the main app xml document using qt's QDomDoc.
 */
class DocumentHandler {
public:
    DocumentHandler& operator=(const DocumentHandler&) = delete;

    static DocumentHandler& getInstance() {
        static DocumentHandler instance;
        return instance;
    }

    /**
     * @brief Adds text node to element with QString value.
     * @param aElement - element to which the text node will be added.
     * @param aValue - Value to be written in the added text node.
     */
    void addTextNode(QDomElement& aElement, const QString& aValue);
    
    /**
     * @brief Creates a root element of specified type in apps QDomDocument under the main root element.
     * @param aRootType - Type of root element to create.
     * @return created QDomElement.
     */
    QDomElement createRootElement(const ItemTypes::Root& aRootType);

    /**
     * @brief Creates a root element of specified type in apps QDomDocument under the main root element.
     * @param aSubType - Type of sub element to create.
     * @param parentElement - QDomElement under which the sub element will be created.
     * @return created QDomElement.
     */
    QDomElement createSubElement(const ItemTypes::Sub& aSubType, QDomElement& aParentElement);

    /**
     * @brief removes the element from QDomDoc.
     * @param aElementToRemove - element that will be removed from the doc.
     */
    void removeElement(QDomElement& aElementToRemove);

    /**
     * @brief Returns Qlist of all elements which text matches the type label.
     * @param aSubType - type of elements to search
     * @return QList<QDomElement> of elements that match the aSubType label.
     */
    QList<QDomElement> getElementsByType(const ItemTypes::Sub& aSubType) const;
    /**
     * @brief Returns root QDomElement of specified type.
     * @param aRootType - type of root element to return.
     * @return QDomElement which text matches the aRootType label.
     */
    QDomElement getRootElement(const ItemTypes::Root& aRootType) const;

    /**
     * @brief Saves current state of QDomDoc to a file in specified filePath.
     * @param aSavePath - path where the doc will be saved.
     */
    void writeDocToXML(const std::string& aSavePath) const;

    /**
     * @brief Returns QDomElement's child element which text equals "Properties". If no such element exist
     * returns empty QDomElement.
     * @param aElement - Element in whose children elements a "Properties" element will be searched.
     * @return Child QDomElement of aElement's that has a text equal to "Properties".
     */
    static QDomElement getProperties(const QDomElement& aElement);

    /**
     * @brief Returns sub element that has text equal to "Property" and an attribute "name" equal to "aPropname".
     * @param aPropertiesElement - parent element under which method willl search for "Property" elements.
     * @param aPropName - value of "name" attribute used in search of the sub element. 
     * @return Child QDomElement of aElement's that has an attribute "name" equal to aPropName and text equal to "Property".
     */
    static QDomElement getProperty(const QDomElement& aPropertiesElement, const QString& aPropName);

    /**
     * @brief Returns text node value of "Property" element that has an attribute "name" equal to "aPropname" 
     * under passed parent element.
     * @param aPropertiesElement - parent element under which method willl search for "Property" elements.
     * @param aPropName - value of "name" attribute used in search of the sub element. 
     * @return text node value of "Property" element that has an attribute "name" equal to "aPropname" 
     */
    static QString getPropertyValue(const QDomElement& aPropertyElement, const QString& aPropName);

    /**
     * @brief Retrieves the value of the specified attribute from a given QDomElement.
     * @param aPropertyElement - The QDomElement from which the attribute value will be retrieved.
     * @param aAttributeName - The name of the attribute whose value is to be fetched.
     * @return The value of the specified attribute as a QString. If the attribute does not exist, an empty string is returned.
     */
    static QString getAttribute(const QDomElement& aPropertyElement, const QString& aAttributeName);

    /**
     * @brief Sets or updates the value of a specified attribute in a given QDomElement.
     * @param aPropertyElement - The QDomElement in which the attribute value will be set or updated.
     * @param aAttributeName - The name of the attribute to be set or updated.
     * @param aNewValue - The new value to assign to the attribute.
     */
    static void setAttribute(QDomElement& aPropertyElement, const QString& aAttributeName, const QString& aNewValue);

    //TODO: add some docUtils in DocumentUtils and move such methods there
    /**
     * @brief Converts a vector of integers into a single QString, with elements separated by commas.
     * @param aIntVec - A vector of integers to be converted into a QString.
     * @return A QString containing the integers from the vector, separated by commas (e.g., "1, 2, 3").
     */
    static QString intsToString(const std::vector<int>& aIntVec);

private:

    DocumentHandler();
    ~DocumentHandler() = default;

    QDomDocument _domDocument;
    QDomElement _appRootElement;

    /**
     * @brief Adds a set of json parsed properties to the specified QDomElement.
     * @param element - The QDomElement to which the properties will be added.
     * @param properties - A collection of properties (as defined by JsonParser::Properties) to add to the element.
     */
    void addPropertiesToElement(QDomElement& element, const JsonParser::Properties& properties);

    /**
     * @brief Searches for a sub-element with a specific tag name under the given QDomElement.
     * @param aElement - The parent QDomElement in which to search for the sub-element.
     * @param aSearchedTag - The tag name of the sub-element to find.
     * @return The first QDomElement with the specified tag name, or an empty QDomElement if no such element is found.
     */
    static QDomElement findSubElement(const QDomElement& aElement, const QString& aSearchedTag);
};

#endif