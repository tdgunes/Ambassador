//
// Created by Taha Doğan Güneş on 27/04/16.
//

#ifndef AMBASSADOR_PROXIMITYSYSTEM_H
#define AMBASSADOR_PROXIMITYSYSTEM_H

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include "Client.h"


class ProximitySystem {

private:
    // key: beacon, value: owner
    std::unordered_map<Beacon, Client *> beaconOwnerMap;

    // key: owner, value set of beacons
    std::unordered_map<Client *, std::unordered_set<Beacon>> ownerBeaconsMap;

public:
    enum class Event : uint8_t {
        LEFT = 0,
        ENTERED = 1,
    };

    void join(Client *client);

    void leave(Client *client);

    void update(ProximitySystem::Event event, Beacon beacon, Client *client);

    std::string prepareEventMessage(Event event, Beacon *beacon, std::string uuid);

    std::string prepareBeaconRegistrationMessage(const Beacon *beacon);

    std::string prepareBeaconLeaveMessage(const Beacon *beacon);

    bool isJoined(Client *client);


    void propagate(Client *client);
};


#endif //AMBASSADOR_PROXIMITYSYSTEM_H
