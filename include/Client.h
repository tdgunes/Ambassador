//
// Created by Taha Doğan Güneş on 10/11/15.
//

#ifndef AMBASSADOR_CLIENT_H
#define AMBASSADOR_CLIENT_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <event2/bufferevent.h>
#include <unordered_set>
#include <stdint.h>
#include <Beacon.h>


class Client {

private:
    std::unordered_set<std::string> following;
    std::unordered_set<Beacon> beacons;

public:
    Client(int fd);

    ~Client();

    enum class Status {
        INIT, UUID, CHAT
    };


    struct evbuffer *buffer;
    uint32_t size;

    int fd;
    Status status;
    std::string uuid;
    struct bufferevent *bufferedEvent = nullptr;

    void send(std::string message);
    void handle();
    void unfollowAll();
    void follow(std::string uuid);

    std::unordered_set<std::string> &getFollowing();

    void addBeacon(Beacon beacon);

    void clearBeacons();

    std::unordered_set<Beacon> &getBeacons();




};


#endif //AMBASSADOR_CLIENT_H
