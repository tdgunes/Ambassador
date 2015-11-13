//
// Created by Taha Doğan Güneş on 10/11/15.
//

#ifndef AMBASSADOR_CLIENT_H
#define AMBASSADOR_CLIENT_H

#include <string>
#include <event2/bufferevent.h>

class Client {


public:
    Client(int fd);

    enum class Status {
        INIT, NICKNAME, CHAT
    };

    int fd;
    Status status;
    std::string nickname;
    struct bufferevent *bufferedEvent;

    void send(std::string message);

    ~Client();


};


#endif //AMBASSADOR_CLIENT_H
