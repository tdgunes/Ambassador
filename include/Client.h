//
// Created by Taha Doğan Güneş on 10/11/15.
//

#ifndef AMBASSADOR_CLIENT_H
#define AMBASSADOR_CLIENT_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <event2/bufferevent.h>


class Client {


public:
    Client(int fd);

    enum class Status {
        INIT, UUID, CHAT
    };


    struct evbuffer *buffer;
    uint16_t size;



    int fd;
    Status status;
    std::string uuid;
    struct bufferevent *bufferedEvent = nullptr;

    void send(std::string message);

    ~Client();


};


#endif //AMBASSADOR_CLIENT_H
