//
// Created by Taha Doğan Güneş on 20/04/16.
//

#ifndef AMBASSADOR_EVENTSYSTEM_H
#define AMBASSADOR_EVENTSYSTEM_H

#include <stdint.h>

class Client;

class EventSystem {
private:


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
};


#endif //AMBASSADOR_EVENTSYSTEM_H
