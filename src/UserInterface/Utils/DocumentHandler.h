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
#ifndef DOCUMENTHANDLER_H
#define DOCUMENTHANDLER_H

#include "JsonParser.h"
#include "AppDefaults.h"

#include <QMap>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>



class DocumentHandler{
    
    public:
        /**
         * Enum used to access the map rootTags where the entries describing root elements in .json template file are stored
         */
        enum class RootTag{
            GeomImport,
            GeomModel,
            CSYS,
            Mesh
        };
        /**
         * Enum used to access the map rootTags where the entries describing root elements in .json template file are stored
         */
        enum class EntryTag{
            MeshSizing,
            STEPImport,
        };
        /**
         * Map that stores strings used to access sub element properties in .json file
         */
        static const QMap<EntryTag, QString> entryTags;

        /**
         * Map that stores strings used to access root element properties in .json file
         */
        static const QMap<RootTag, QString> rootTags;

        DocumentHandler();
        ~DocumentHandler();

        /**
         * @brief write application document to .xml file.
         * @param savePath path where the .xml document will be saved
         */
        void writeDocToXML(const std::string& savePath);

        /**
         * @brief Creates a new element under main application element in the document. New element will have 
         * Properties sub element with properties based on the ProjectSetup.json file. The .json is parsed  based 
         * on the RootTag input.
         * @param newElementTag RootTag enum of the element that will be created.
         */
        QDomElement createRootElement(const RootTag& newElementTag);
        /**
         * @brief Creates a new element under given parent element in the document.New element will have 
         * Properties sub element with properties based on the ProjectSetup.json file. The .json is parsed  based 
         * on the EntryTag input.
         * @param newElementTag EntryTag enum of the element that will be created.
         * @param parentElement Parent QDomElement under which the new element will be added.
         */
        QDomElement createSubElement(const EntryTag& newElementTag,
                                        QDomElement& parentElement);
        /**
         * @brief Returns a <Properties> sub element of the provided element.
         * @param element Properties sub element of this input will be returned.
         * @return Properties QDomElement.
         */
        QDomElement getElementsPropertiesElement(const QDomElement& element);
        /**
         * @brief Returns a Qlist of elements in main document that match the input entryTagd
         * @param entryTag EntryTag enum - elements that match this tag will be returned. 
         * @return QList<QDomElemtn> list of elements that match the input entryTag.
         */
        QList<QDomElement> getElementsByTag(const EntryTag& entryTag);

        /**
         * @brief Returns a root element that exists in main document and matches the RootTag input
         */
        QDomElement getRootElement(const RootTag& rootTag);

        /**
         * @brief returns the value of a property (subText of a node) as a QString
         */
        QString getPropertyValue(const QDomElement& element, const QString& propName);

    private:
        /**
         * Main document of the application 
         */
        QDomDocument _domDocument;
        /**
         * Main root element in the _domDocument. 
         */
        QDomElement _appRootElement;

        /**
         * @brief Adds <Properties> element to the input element and appends <property> sub elements for each property map
         * specified in PropertiesList.
         * @param element Element to which the <Properties> element will be appended.
         * @param propertiesList QList<QMap<QString,QString> a list of properties to add to the element.\\
         */
        void addPropertiesToElement(QDomElement& element, const PropertiesList& propertiesList);
};




#endif