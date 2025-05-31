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

#ifndef HANDLERLIST_HPP 
#define HANDLERLIST_HPP 

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

struct HandlerListBase {
        virtual ~HandlerListBase() = default;
};

template<typename MessageType>
struct HandlerList : HandlerListBase {
    std::vector<std::function<void(const MessageType&)>> handlers;

    void push_back(std::function<void(const MessageType&)> h) {
        handlers.push_back(std::move(h));
    }

    void invokeAll(const MessageType& msg) const {
        for (const auto& h : handlers) {
            h(msg);
        }
    }
};

#endif