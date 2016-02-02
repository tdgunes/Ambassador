//
// Created by Taha Doğan Güneş on 10/11/15.
//

#include <iostream>
#include <event2/buffer.h>
#include "Client.h"

Client::Client(int fd) {
    this->fd = fd;
    this->status = Client::Status::INIT;
    this->buffer = evbuffer_new();
    this->size = 0;
}

void Client::send(std::string message) {
    message = message + std::string("\n");
    if (message.length() > 0) {
        u_short s = htons(strlen(message.c_str()));
        std::cout << "Departure Package Size: " << strlen(message.c_str()) << std::endl;
        bufferevent_write(this->bufferedEvent, &s, 2);
        bufferevent_write(this->bufferedEvent, message.c_str(), strlen(message.c_str()));
    }
    else {
        std::cout << "Tried to send empty message" << std::endl;
    }

}

Client::~Client() {
    delete buffer;
}


