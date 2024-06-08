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
#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "document.h"
#include <string>
#include <QDebug>
#include <QFile>
#include <QtXml/QDomElement>
#include <vtkLogger.h>

class JsonParser{
    
    public:
        /**
         * Create rapidjson::Document object based on the path to .json file
         * @param filePath path to .json file
         */
        rapidjson::Document initJsonDocumnet(const QString& filePath);
        
        /**
         * Parse top-level entry in the rapidjson::Document and create a QDomElement based on it.
         * @param document rapidjson::Document to be parsed. Can be obtained using initJsonDocument method
         * @param entryName Name of the top-level entry to be parsed.
         */
        QDomElement parseEntryProperties(const rapidjson::Document& document, const QString& entryTag);
        
        /**
         * Parses entire json Document calling parseJsonDocumentEntry for each entry
         * @param document rapidjson::Document to be parsed. Can be obtained using initJsonDocument method
         */
        QDomElement parseJsonDocument(const rapidjson::Document& document);

    private:
        /**
         * Helper method that creates a QDomElement based on rapidjson::array
         * @param array
         */
        QDomElement createElementFromValue(const rapidjson::Value& jsonLine, const QString& newElementTag);
};
#endif