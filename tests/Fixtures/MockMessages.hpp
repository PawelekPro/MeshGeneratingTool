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

#include "BaseMessage.hpp"
#include <vector>


class MockMessage : public BaseMessage {
    public:
    MockMessage(int id) : _id(id) {};
    int _id;
};

class MockEvent : public BaseMessage {
    public:
    MockEvent(int id) : _id(id) {};
    int _id;
};

class MockRequest : public BaseMessage {
    public:
    MockRequest(int id) : _id(id) {};
    int _id;
};

class MessageTracker {
    public:
    MessageTracker() = default;
    ~MessageTracker() = default;

    void onMessagePublished(const MockMessage& aMessage){
        calledMessageIds.push_back(aMessage._id);
    }
    int nMessagesPublished(){return calledMessageIds.size();}

    void onEventPublished(const MockEvent& aEvent){
        calledEventIds.push_back(aEvent._id);
    }
    int nEventsPublished(){return calledEventIds.size();}
    
    void onRequestPublished(const MockRequest& aRequest){
        calledRequestIds.push_back(aRequest._id);
    }
    int nRequestsPublished(){return calledRequestIds.size();}

    std::vector<int> calledMessageIds;
    std::vector<int> calledEventIds;
    std::vector<int> calledRequestIds;
};