//
// Created by Taha Doğan Güneş on 20/04/16.
//

#include <Client.h>
#include <json.hpp>
#include <Server.h>
#include "EventSystem.h"

void EventSystem::join(Client *client) {
    // a client which wants to follow some other clients' network activities

    auto followings = client->getFollowing();

    followerMap[client] = followings;

    for (std::string uuid : followings) {
        if (followedMap.count(uuid) == 0) {
            std::unordered_set<Client *> set;
            followedMap[uuid] = set;

        }

        Event event = Server::uuids.count(uuid) > 0 ? Event::CONNECTED : Event::OFFLINE;
        client->send(this->prepareEventMessage(event, uuid));

        followedMap[uuid].insert(client);
    }


}

void EventSystem::leave(Client *client) {
    // a clients that wants to leave the event system

    followerMap.erase(client);

    auto followings = client->getFollowing();
    for (std::string uuid: followings) {
        followedMap.erase(uuid);
    }
}

void EventSystem::update(Event event, Client *client) {
    for (Client *follower: followedMap[client->uuid]) {
        // follower wants to know what happened to the client
        follower->send(this->prepareEventMessage(event, client->uuid));
    }
}

bool EventSystem::isJoined(Client *client) {
    return followedMap.count(client->uuid) > 0;
}

std::string EventSystem::prepareEventMessage(Event event, std::string uuid) {
    nlohmann::json package;

    switch (event) {
        case Event::CONNECTED:
            package["event"] = 1;
            break;
        case Event::LEAVING:
            package["event"] = 0;
            break;
        case Event::OFFLINE:
            package["event"] = 2;
            break;
    }
    package["category"] = "event";
    package["client"] = uuid;
    return package.dump();
}









