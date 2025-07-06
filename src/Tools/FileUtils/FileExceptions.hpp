/*
* Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef FILEEXCEPTIONS_HPP
#define FILEEXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace Exceptions{
namespace File {

    class FileException : public std::runtime_error {
    public:
        explicit FileException(const std::string& message)
            : std::runtime_error(message) {}
    };

    class FileDoesNotExist : public FileException {
    public:
        explicit FileDoesNotExist(const std::string& filePath)
            : FileException("File does not exist: " + filePath) {}
    };

};
};

#endif