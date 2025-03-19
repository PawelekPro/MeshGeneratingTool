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
* File      : DocumentHandler.hpp
* Author    : Paweł Gilewicz, Krystian Fudali
* Date      : 09/03/2025
*/

#ifndef DOCUMENTHANDLER_HPP
#define DOCUMENTHANDLER_HPP

#include "AppInfo.hpp"
#include "DefaultsParser.hpp"
#include "DocItemTypes.hpp"

#include <QMap>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// TODO: Decouple DocumentHandler from AppInfo somehow. Think of better way to
// store paths
//       We would like this to be completely independet module for easier
//       testing.

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
	 * @brief Creates a root element of specified type in apps QDomDocument
	 * under the main root element.
	 * @param aRootType - Type of root element to create.
	 * @return created QDomElement.
	 */
	QDomElement createRootElement(const ItemTypes::Root& aRootType);

	/**
	 * @brief Creates a root element of specified type in apps QDomDocument
	 * under the main root element.
	 * @param aSubType - Type of sub element to create.
	 * @param parentElement - QDomElement under which the sub element will be
	 * created.
	 * @return created QDomElement.
	 */
	QDomElement createSubElement(
		const ItemTypes::Sub& aSubType, QDomElement& aParentElement);

	/**
	 * @brief Returns Qlist of all elements which text matches the type label.
	 * @param aSubType - type of elements to search
	 * @return QList<QDomElement> of elements that match the aSubType label.
	 */
	QList<QDomElement> getSubElements(const ItemTypes::Sub& aSubType) const;
	/**
	 * @brief Returns root QDomElement of specified type.
	 * @param aRootType - type of root element to return.
	 * @return QDomElement which text matches the aRootType label.
	 */
	QDomElement getRootElement(const ItemTypes::Root& aRootType) const;

	/**
	 * @brief removes the element from QDomDoc.
	 * @param aElementToRemove - element that will be removed from the doc.
	 */
	void removeElement(QDomElement& aElementToRemove);

	/**
	 * @brief Adds text node to element with QString value.
	 * @param aElement - element to which the text node will be added.
	 * @param aValue - Value to be written in the added text node.
	 */
	void addTextNode(QDomElement& aElement, const QString& aValue);

	/**
	 * @brief Saves current state of QDomDoc to a file in specified filePath.
	 * @param aSavePath - path where the doc will be saved.
	 */
	void writeDocToXML(const std::string& aSavePath) const;

	[[nodiscard]] QMap<QString, QString> getPropertyNodeMap(
		const ItemTypes::Root& aRootType) const;

private:
	DocumentHandler();
	~DocumentHandler() = default;

	QDomDocument _domDocument;
	QDomElement _appRootElement;
};

#endif