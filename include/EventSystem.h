//
// Created by Taha Doğan Güneş on 20/04/16.
//

#ifndef AMBASSADOR_EVENTSYSTEM_H
#define AMBASSADOR_EVENTSYSTEM_H

#include <stdint.h>
#include <unordered_map>
#include <unordered_set>

class Client;

class EventSystem {

public:
    enum class Event : uint8_t {
        LEAVING = 0,
        CONNECTED = 1,
        OFFLINE = 2,
    };

    // for listening or leaving event system
    void join(Client *client);
    void leave(Client *client);

    bool isJoined(Client *client);

    // give inputs to system to handle the events
    void update(EventSystem::Event event, Client *client);

private:
    // key: client that is followed, value: set of followers
    std::unordered_map<std::string, std::unordered_set<Client *>> followedMap;

    // key: follower, value set of clients that are followed
    std::unordered_map<Client *, std::unordered_set<std::string>> followerMap;

    std::string prepareEventMessage(Event event, std::string uuid);



};


#endif //AMBASSADOR_EVENTSYSTEM_H
