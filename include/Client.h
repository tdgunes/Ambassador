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

    int headerBytes = 0;
    int bufferPointer = 0;
    int bytesLeft = 0;
    char buffer[65536];
    int messageLength;
    union len {
        unsigned char length_c[2];
        short length_s;
    } length;


    int fd;
    Status status;
    std::string uuid;
    struct bufferevent *bufferedEvent = nullptr;

    void send(std::string message);

    ~Client();


};


#endif //AMBASSADOR_CLIENT_H
