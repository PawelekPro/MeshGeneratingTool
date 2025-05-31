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

#ifndef MESSAGEBUS_HPP 
#define MESSAGEBUS_HPP 

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <functional>
#include "HandlerList.hpp"
#include "BaseMessage.hpp"

class MessageBus {
public:
    template<typename MessageType>
    void subscribe(std::function<void(const MessageType&)> handler) {
        static_assert(std::is_base_of<BaseMessage, MessageType>::value,
                      "MessageType must derive from BaseMessage");
        auto messageTypeId = std::type_index(typeid(MessageType));
        
        if (_handlers.find(messageTypeId) == _handlers.end()){
            _handlers[messageTypeId] = std::make_shared<HandlerList<MessageType>>();
        }
        
        auto list = std::static_pointer_cast<HandlerList<MessageType>>(
            _handlers[messageTypeId]
        );
        
        list->push_back(std::move(handler));
    }

    template<typename MessageType>
    void publish(const MessageType& message) const {
        static_assert(std::is_base_of<BaseMessage, MessageType>::value,
                      "MessageType must derive from BaseMessage");
        auto it = _handlers.find(std::type_index(typeid(MessageType)));
        if (it != _handlers.end()) {
            auto list = std::static_pointer_cast<HandlerList<MessageType>>(it->second);
            list->invokeAll(message);
        }
    }
private:
    std::unordered_map<std::type_index, std::shared_ptr<HandlerListBase>> _handlers;
};


#endif // MESSAGEBUS_HPP
