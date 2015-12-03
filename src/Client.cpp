//
// Created by Taha Doğan Güneş on 10/11/15.
//

#include <iostream>
#include "Client.h"

Client::Client(int fd) {
    this->fd = fd;
    this->status = Client::Status::INIT;
}

void Client::send(std::string message) {
    if (message.length() > 0)
        bufferevent_write(this->bufferedEvent, message.c_str(), strlen(message.c_str()));
    else
        std::cout << "Tried to send empty message" << std::endl;
}

Client::~Client() {

}


