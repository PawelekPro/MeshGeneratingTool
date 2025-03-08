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

#ifndef PROGRESSOBSERVER_HPP
#define PROGRESSOBSERVER_HPP

#include <functional>
#include <string>
#include "EventObserver.hpp"

class ProgressObserver : public EventObserver {

    using ProgressCallback = std::function<void(const std::string&, int)>;
    
    public:

    void setProgressCallback(ProgressCallback aProgressCallback);

    private:

    void visit(const ProgressEvent&) override;
    ProgressCallback _progressCallback = [](const std::string&, int) {};
};

#endif