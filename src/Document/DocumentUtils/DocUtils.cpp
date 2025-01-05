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
#include "DocUtils.hpp"

#include <QDebug>

namespace DocUtils{

    QDomElement getSubElement(const QDomElement& aParentElement, const QString& aSearchedElementText) {
        if (aParentElement.isNull()) {
            qWarning("Invalid element passed to getSubElement. Returning empty QDomElement.");
            return QDomElement();
        }

        QDomNodeList childElements = aParentElement.elementsByTagName(aSearchedElementText);

        if (childElements.count() == 0) {
            qWarning() << "No elements with tag name" << aSearchedElementText 
                    << "found under parent element" << aParentElement.tagName() << ".";
        } else if (childElements.count() > 1) {
            qWarning() << "Multiple elements with tag name" << aSearchedElementText 
                    << "found under parent element" << aParentElement.tagName() 
                    << ". Returning the first matching element.";
            return childElements.at(0).toElement();
        } else {
            return childElements.at(0).toElement();
        }
        
        return QDomElement();
    }
}

namespace Properties{

    QDomElement getProperty(const QDomElement& aParentElement, const QString& aPropertyName) {
        if (aParentElement.isNull()) {
            qWarning() << "Invalid element passed to getProperty. Returning empty QDomElement.";
            return QDomElement();
        }

        QDomElement propertiesElement;
        if (aParentElement.tagName() == "Properties") {
            propertiesElement = aParentElement;
        } else {
            propertiesElement = DocUtils::getSubElement(aParentElement, QString("Properties"));
        }

        if (propertiesElement.isNull()) {
            qWarning() << "No <Properties> element found under parent element" 
                    << aParentElement.tagName() << ". Returning empty QDomElement.";
            return QDomElement();
        }

        QDomNodeList propertyNodes = propertiesElement.elementsByTagName("Property");
        for (int i = 0; i < propertyNodes.count(); ++i) {
            QDomElement propertyElement = propertyNodes.at(i).toElement();
            if (
                propertyElement.hasAttribute("name") &&
                propertyElement.attribute("name") == aPropertyName) {
                return propertyElement;
            }
        }

        qWarning() << "No <Property> element with name" << aPropertyName 
                << "found in <Properties> element under parent element" 
                << aParentElement.tagName() << ". Returning empty QDomElement.";
        return QDomElement();
    }

    QString getPropertyValue(const QDomElement& aParentElement, const QString& aPropertyName){
        QDomElement property = getProperty(aParentElement, aPropertyName);
        return property.text();
    }



}