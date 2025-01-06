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

    bool isProperty(const QDomElement& aElement){
        QDomElement parent = aElement.parentNode().toElement();
        if(aElement.isNull()){
            return false;
        }
        if(aElement.tagName() != "Property"){
            return false;
        }
        return true;
    }

    QString getPropertyValue(const QDomElement& aProperty){
        if(isProperty(aProperty)){
            return aProperty.text();
        } else {
            qWarning() << "Element is not a property, returning empty QString";
            return QString();
        }
    }

    void setPropertyValue(QDomElement& aProperty, const QString& newValue) {
        if (isProperty(aProperty)) {
            while (!aProperty.firstChild().isNull()) {
                aProperty.removeChild(aProperty.firstChild());
            }
            QDomText textNode = aProperty.ownerDocument().createTextNode(newValue);
            aProperty.appendChild(textNode);
        } else {
            qWarning() << "Property is invalid. Could not set value to: " << newValue;
        }
    }

    QString getPropertyAttribute(const QDomElement& aProperty, const QString& aAttribute) {
        if (isProperty(aProperty)) {
            if (aProperty.hasAttribute(aAttribute)) {
                return aProperty.attribute(aAttribute);
            } else {
                qWarning() << "Property does not have the requested attribute:" << aAttribute;
                return QString();
            }
        } else {
            qWarning() << "Invalid property element passed to getPropertyAttribute.";
            return QString();
        }
    }

    void setPropertyAttribute(QDomElement& aProperty, const QString& aAttribute, const QString& aNewValue) {
        if (isProperty(aProperty)) {
            aProperty.setAttribute(aAttribute, aNewValue);
        } else {
            qWarning() << "Could not set attribute:" << aAttribute << "to value:" << aNewValue << "- Invalid property element";
        }
    }
} 