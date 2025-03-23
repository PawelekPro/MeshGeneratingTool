/*
* Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
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

*=============================================================================
* File      : DocumentHandler.cpp
* Author    : Paweł Gilewicz, Krystian Fudali
* Date      : 09/03/2025
*/

#include "DocumentHandler.hpp"

#include "DisplayRulesParser.hpp"

#include <DocUtils.hpp>

//----------------------------------------------------------------------------
DocumentHandler::DocumentHandler() {
	this->_appRootElement
		= this->_domDocument.createElement(AppInfo::getInstance().getAppName());

	this->_appRootElement.setAttribute(
		"version", AppInfo::getInstance().getAppProjFileVersion());

	this->_domDocument.appendChild(this->_appRootElement);
}

//----------------------------------------------------------------------------
void DocumentHandler::writeDocToXML(const std::string& savePath) const {
	QFile file(QString::fromStdString(savePath));
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Failed to open file for writing.";
		return;
	}

	QTextStream out(&file);
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	// Indentation of 4 spaces
	this->_domDocument.save(out, 4);

	file.close();
}

//----------------------------------------------------------------------------
QDomElement DocumentHandler::createRootElement(
	const ItemTypes::Root& aRootType) {

	const QString rootTagLabel = ItemTypes::label(aRootType);
	QDomElement rootElement = this->_domDocument.createElement(rootTagLabel);
	const QDomElement properties
		= DefaultsParser::getItemProperties(_domDocument, aRootType);

	auto displayRulesMap = DisplayRulesParser::parseVisibilityRules(
		AppInfo::visibilityRulesPath);

	// Add visibility rules if exist
	const QDomNodeList propertyNodes = properties.elementsByTagName("Property");
	for (int i = 0; i < propertyNodes.count(); ++i) {
		QDomElement property = propertyNodes.at(i).toElement();

		if (QString propertyName = property.attribute("name");
			displayRulesMap.contains(propertyName)) {
			// Create VisibilityRules element and add it as child to property
			QDomElement displayRules
				= _domDocument.createElement("DisplayRules");

			for (const QString& hiddenProperty :
				displayRulesMap[propertyName]) {
				QDomElement hideElement = _domDocument.createElement("Hide");
				hideElement.setAttribute("name", hiddenProperty);
				displayRules.appendChild(hideElement);
			}
			property.appendChild(displayRules);
		}
	}

	rootElement.appendChild(properties);
	this->_appRootElement.appendChild(rootElement);
	return rootElement;
}

//----------------------------------------------------------------------------
QDomElement DocumentHandler::createSubElement(
	const ItemTypes::Sub& aSubType, QDomElement& aParentElement) {
	if (!aParentElement.isElement()) {
		qWarning("Root element does not exist");
	}
	const QString& subTagLabel = ItemTypes::label(aSubType);
	QDomElement element = this->_domDocument.createElement(subTagLabel);
	const QDomElement properties
		= DefaultsParser::getItemProperties(_domDocument, aSubType);

	element.appendChild(properties);
	aParentElement.appendChild(element);
	return element;
}

//----------------------------------------------------------------------------
QList<QDomElement> DocumentHandler::getSubElements(
	const ItemTypes::Sub& aSubType) const {
	const QDomNodeList nodeList
		= _domDocument.elementsByTagName(ItemTypes::label(aSubType));
	QList<QDomElement> elementList;
	for (int i = 0; i < nodeList.size(); ++i) {
		if (QDomNode node = nodeList.at(i); node.isElement()) {
			elementList.append(node.toElement());
		}
	}
	return elementList;
}

//----------------------------------------------------------------------------
QDomElement DocumentHandler::getRootElement(
	const ItemTypes::Root& rootTag) const {
	const QDomNodeList nodeList
		= this->_domDocument.elementsByTagName(ItemTypes::label(rootTag));
	QDomElement rootElement = nodeList.at(0).toElement();
	return rootElement;
}

//----------------------------------------------------------------------------
void DocumentHandler::removeElement(const QDomElement& aElementToRemove) {
	if (QDomNode parentNode = aElementToRemove.parentNode();
		parentNode.isElement()) {
		QDomElement parentElement = parentNode.toElement();
		parentElement.removeChild(aElementToRemove);
		return;
	}
}

//----------------------------------------------------------------------------
void DocumentHandler::addTextNode(
	QDomElement& aElement, const QString& aValue) {
	const QDomText valueNode = _domDocument.createTextNode(aValue);
	aElement.appendChild(valueNode);
}

//----------------------------------------------------------------------------
QMap<QString, QString> DocumentHandler::getPropertyNodeMap(
	const ItemTypes::Root& aRootType) const {
	const QDomElement elem = getRootElement(aRootType);
	return Properties::getPropertyNodeMap(elem);
}