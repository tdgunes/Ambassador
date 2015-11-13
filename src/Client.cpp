//
// Created by Taha Doğan Güneş on 10/11/15.
//

#include "Client.h"

Client::Client(int fd) {
    this->fd = fd;
    this->status = Client::Status::INIT;
}

void Client::send(std::string message) {
    bufferevent_write(this->bufferedEvent, message.c_str(), strlen(message.c_str()));
}

Client::~Client() {

}


