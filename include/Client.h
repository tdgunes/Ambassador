//
// Created by Taha Doğan Güneş on 10/11/15.
//

#ifndef AMBASSADOR_CLIENT_H
#define AMBASSADOR_CLIENT_H

#include <string>
#include <event2/bufferevent.h>

class Client {


public:
    int fd;
    struct bufferevent *bufferedEvent;

    Client(int fd);

    enum class Status {
        INIT, NICKNAME, CHAT
    };
    Status status;
    std::string nickname;

    ~Client();


};


#endif //AMBASSADOR_CLIENT_H
