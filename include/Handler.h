//
// Created by Taha Doğan Güneş on 13/11/15.
//

#ifndef AMBASSADOR_HANDLER_H
#define AMBASSADOR_HANDLER_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <event2/bufferevent.h>


#include "Client.h"


class Handler {

public:
    Handler();

    void handleMessage(Client *from, std::string &message);

    void onUUID(Client *from, std::string &message);

    void onChat(Client *from, std::string &message);

    ~Handler();


};


#endif //AMBASSADOR_HANDLER_H
