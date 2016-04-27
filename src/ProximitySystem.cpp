//
// Created by Taha Doğan Güneş on 27/04/16.
//

#include <json.hpp>
#include <Beacon.h>
#include "ProximitySystem.h"

void ProximitySystem::join(Client *client) {
    auto beacons = client->getBeacons();

    for (auto beacon : beacons) {
        beaconOwnerMap[beacon] = client;
    }

    ownerBeaconsMap[client] = beacons;
}

void ProximitySystem::leave(Client *client) {
    ownerBeaconsMap.erase(client);
    auto beacons = client->getBeacons();

    for (auto beacon : beacons) {
        beaconOwnerMap.erase(beacon);
    }
}

void ProximitySystem::update(ProximitySystem::Event event, Beacon beacon, Client *client) {
    // "client" fired an "event" that is related to a "beacon".
    auto owner = beaconOwnerMap[beacon];
    owner->send(this->prepareEventMessage(event, client->uuid));
}

std::string ProximitySystem::prepareEventMessage(Event event, std::string uuid) {
    nlohmann::json package;

    switch (event) {
        case Event::ENTERED:
            package["event"] = 1;
            break;
        case Event::LEFT:
            package["event"] = 0;
            break;
    }
    package["category"] = "proximity";
    package["client"] = uuid;
    return package.dump();
}








