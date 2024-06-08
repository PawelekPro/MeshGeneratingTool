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
        enum class RootTag{
            GeomImport,
            GeomModel,
            CSYS,
            Mesh
        };
        enum class EntryTag{
            MeshSizing,
            STEPImport,
        };
        static const QMap<EntryTag, QString> entryTags;
        static const QMap<RootTag, QString> rootTags;

        void writeDocToXML(const std::string& savePath);
    private:
        /**
         * Main document of the application 
         */
        QDomDocument _domDocument;

        void initializeDocument();

        void addRootElement(const rapidjson::Document& document, RootTag& rootTag);

        void addPropertiesToElement(QDomElement& element, const PropertiesList& propertiesList);

        
};




#endif