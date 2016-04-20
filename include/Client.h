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


class Client {

private:
    std::unordered_set<std::string> following;

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



};


#endif //AMBASSADOR_CLIENT_H
