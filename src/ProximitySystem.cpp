//
// Created by Taha Doğan Güneş on 27/04/16.
//

#include <json.hpp>
#include <Beacon.h>
#include "ProximitySystem.h"

void ProximitySystem::join(Client *client) {
    auto beacons = client->getBeacons();

    for (auto beacon : beacons) {
        std::cout << "[ProximitySystem]" << client->uuid << " registered: " << beacon.major << " : " << beacon.minor <<
        std::endl;
        beaconOwnerMap[beacon] = client;
    }

    ownerBeaconsMap[client] = beacons;
}

void ProximitySystem::leave(Client *client) {
    ownerBeaconsMap.erase(client);
    auto beacons = client->getBeacons();

    for (auto beacon : beacons) {
        std::cout << "[ProximitySystem]" << client->uuid << " unregistered: " << beacon.major << " : " <<
        beacon.minor << std::endl;
        beaconOwnerMap.erase(beacon);
    }
}

void ProximitySystem::update(ProximitySystem::Event event, Beacon beacon, Client *client) {
    // "client" fired an "event" that is related to a "beacon".
    if (beaconOwnerMap.count(beacon) > 0) {
        auto owner = beaconOwnerMap[beacon];
        owner->send(this->prepareEventMessage(event, &beacon, client->uuid));
    }
    else {
        std::cout << "[ProximitySystem]" << "Unable to find owner of the beacon: " << beacon.major << ":" <<
        beacon.minor << " with event: " << (int) event << std::endl;
    }

}

std::string ProximitySystem::prepareEventMessage(Event event, Beacon *beacon, std::string uuid) {
    nlohmann::json package;
    nlohmann::json b;
    switch (event) {
        case Event::ENTERED:
            package["event"] = 1;
            break;
        case Event::LEFT:
            package["event"] = 0;
            break;
    }
    package["request"] = "proximity";

    b["major"] = beacon->major;
    b["minor"] = beacon->minor;
    package["beacon"] = b;
    package["client"] = uuid;
    return package.dump();
}

bool ProximitySystem::isJoined(Client *client) {
    return ownerBeaconsMap.count(client) > 0;
}










