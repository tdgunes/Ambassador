//
// Created by Taha Doğan Güneş on 10/11/15.
//

#include <iostream>
#include <event2/buffer.h>
#include <Server.h>
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
        std::cout << "Departure Package Size: " << strlen(message.c_str()) << std::endl;
        uint32_t s = htonl(strlen(message.c_str()));

        bufferevent_write(this->bufferedEvent, &s, 4);
        bufferevent_write(this->bufferedEvent, message.c_str(), strlen(message.c_str()));
    }
    else {
        std::cout << "Tried to send empty message" << std::endl;
    }
}

Client::~Client() {
    evbuffer_free(this->buffer);
}


void Client::handle() {
    while ((size == 0 && evbuffer_get_length(buffer) >= 4) ||
           (size > 0 && evbuffer_get_length(buffer) >= size)
            ) {

        if (size == 0 && evbuffer_get_length(buffer) >= 4) {
            int drained = evbuffer_remove(buffer, &size, 4);
            size = ntohl(size);
            std::cout << size << std::endl;
            std::cout << "Drained:" << drained << std::endl;
        }

        if (size > 0 && evbuffer_get_length(buffer) >= size) {
            uint8_t message[size];
            evbuffer_remove(buffer, &message, size);
            std::string str_message((char *) message, size);
            Server::handler->handleMessage(this, str_message);
            size = 0;
        }

    }

}
