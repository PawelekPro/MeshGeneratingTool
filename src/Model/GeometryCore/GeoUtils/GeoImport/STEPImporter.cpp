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

#include "STEPOcafImporter.hpp"
#include "STEPCAFControl_Reader.hxx"
#include <stdexcept>
#include "OccProgressWrapper.hpp"

STEPOcafImporter::STEPOcafImporter(const IEventSubject& aModelSubject) : 
    OcafImporter(aModelSubject){}

bool STEPOcafImporter::importToDocument(const std::string& aFilePath, Handle(TDocStd_Document) aDestDoc) {
    if (!filePathExists(aFilePath)) {
        return false;
    }

    STEPCAFControl_Reader cafReader;
    cafReader.SetColorMode(true);
    cafReader.SetLayerMode(true);
    cafReader.SetNameMode(true);

    IFSelect_ReturnStatus result = cafReader.ReadFile(aFilePath.c_str());
    if (result != IFSelect_RetDone) {
        throw std::runtime_error("Failed to read STEP file.");
    }
    OccProgressWrapper progressWrapper(_modelSubject, aFilePath);
    return cafReader.Transfer(aDestDoc, progressWrapper.Start());
}