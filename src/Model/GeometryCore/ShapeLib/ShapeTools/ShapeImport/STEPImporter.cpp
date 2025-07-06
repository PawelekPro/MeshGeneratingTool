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
#include "STEPImporter.hpp"
#include "OccProgressIndicator.hpp"
#include <stdexcept>

#include <STEPCAFControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <Quantity_ColorRGBA.hxx>
#include <XmlXCAFDrivers.hxx>

Handle(TDocStd_Document) STEPImporter::import(
    const std::string& aFilePath,
    const ProgressIndicator& aProgressIndicator
) const {
    OccProgressIndicator progressIndicator(aProgressIndicator);

    auto fileStream  = FileUtils::readFile(aFilePath);

    STEPCAFControl_Reader reader;
    reader.SetColorMode(true);
    reader.SetNameMode(true);

    IFSelect_ReturnStatus result = reader.ReadStream(
        "STEPFileStream", 
        *fileStream
    );

    if (result != IFSelect_RetDone) {
        throw Exceptions::File::FileException("Could not read STEP file");
    }

    auto document = initDocument();
    reader.Transfer(document, progressIndicator.Start());

    return document;
}