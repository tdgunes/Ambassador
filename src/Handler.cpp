//
// Created by Taha Doğan Güneş on 13/11/15.
//

#include <Server.h>
#include "Handler.h"


Handler::Handler() {

}

void Handler::handleMessage(Client *from, std::string message) {
    if (from == nullptr)
        throw std::invalid_argument("Received a null client as parameter.");

    switch (from->status) {
        case Client::Status::CHAT:
            this->onNickname(from, message);
            break;

        case Client::Status::NICKNAME:
            this->onChat(from, message);
            break;

        default:
            throw std::invalid_argument("Client is in an unknown state.");
    }
}


void Handler::onNickname(Client *from, std::string message) {
    std::cout << "[" << from->nickname << "]: " << message << std::endl;
    for (auto pair: Server::clients) {
        Client *client = pair.second;
        if (client != from) { // do not send me same message back
            std::string cargo = "[" + from->nickname + "]: " + message + "\n";
            from->send(cargo);
        }
    }
}

void Handler::onChat(Client *from, std::string message) {
    from->nickname = message;
    std::cout << "Client from " << from->fd << ", now is " << from->nickname << "." << std::endl;
    from->status = Client::Status::CHAT;
}

Handler::~Handler() {

}


