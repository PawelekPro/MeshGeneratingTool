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

#include "GeoImporter.hpp"
#include <filesystem>

OcafImporter::OcafImporter(const IEventSubject& aModelSubject) : 
    _modelSubject(aModelSubject){}

bool OcafImporter::filePathExists(const std::string& aFilePath){
    if (!std::filesystem::exists(aFilePath)) {
        auto message = "File " + aFilePath + " can not be found.";
        auto errorCode = std::make_error_code(std::errc::no_such_file_or_directory);
        throw std::filesystem::filesystem_error(message, errorCode);
        return false;
    } else {
        return true;
    }
}