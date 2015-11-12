//
// Created by Taha Doğan Güneş on 10/11/15.
//

#include "Client.h"

Client::Client(int fd) {
    this->fd = fd;
    this->status = Client::Status::INIT;
}

Client::~Client() {

}
