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
#ifndef DOCUTILS_HPP
#define DOCUTILS_HPP

#include "document.h"

#include <QDomElement>
#include <QString>

#include <vector>

namespace DocUtils{

    QDomElement getSubElement(const QDomElement& aParentElement, const QString& aSearchedElementText);

    QString intsToString(const std::vector<int>& aIntVec);
    
}

namespace Properties{

    QDomElement getProperty(const QDomElement& aParentElement, const QString& aPropertyName);
    bool isProperty(const QDomElement& aProperty);

    QString getPropertyValue(const QDomElement& aProperty);
    void setPropertyValue(QDomElement& aProperty, const QString& aNewValue);

    QString getPropertyAttribute(const QDomElement& aProperty, const QString& aAttribute);
    void setPropertyAttribute(QDomElement& aProperty, const QString& aAttribute, const QString& aNewValue);
}

#endif