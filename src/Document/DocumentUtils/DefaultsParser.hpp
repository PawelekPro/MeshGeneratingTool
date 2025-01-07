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

#ifndef DEFAULTSPARSER_HPP
#define DEFAULTSPARSER_HPP

#include "DocItemTypes.hpp"
#include "ComboBoxTypes.hpp"
#include "document.h"

#include <QDomElement>
#include <QStringList>

namespace DefaultsParser {

    QDomElement getItemProperties(QDomDocument& aDomDoc, const ItemTypes::Root& aItemType);
    QDomElement getItemProperties(QDomDocument& aDomDoc, const ItemTypes::Sub& aItemType);
    QStringList getComboBoxList(const ComboBoxTypes& aComboBoxType);

    QDomElement getPropertiesFromJson(QDomDocument& aDomDoc, const rapidjson::Document& aDoc, const QString& aParentEntry);
    QDomElement jsonValueToProperty(QDomDocument& aDomDoc, const rapidjson::Value& aJsonPropValue);
    QStringList getComboBoxListFromJson(const rapidjson::Document& aJsonDoc, const QString& aComboBoxEntry);
};

#endif