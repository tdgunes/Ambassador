//
// Created by Taha Doğan Güneş on 13/11/15.
//

#include "Handler.h"
#include "json.hpp"
#include "Server.h"
#include <algorithm>

using json = nlohmann::json;

Handler::Handler() {


}

void Handler::handleMessage(Client *from, std::string message) {
    if (from == nullptr)
        throw std::invalid_argument("Received a null client as parameter.");

    message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
    message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
    std::cout << "[>>>\n" << message << "\n<<<]" << std::endl;
    switch (from->status) {
        case Client::Status::CHAT:
            this->onChat(from, message);
            break;

        case Client::Status::UUID:

            this->onUUID(from, message);
            break;

        default:
            throw std::invalid_argument("Client is in an unknown state.");
    }
}


void Handler::onChat(Client *from, std::string message) {

    std::cout << "[" << from->uuid << "]: " << message << std::endl;


    try {
        auto jsonObject = json::parse(message);
        std::string uuid = jsonObject["uuid"];


        if (uuid.length() > 0) {
            if (Server::uuids.find(uuid) != Server::uuids.end()) {
                Client *to = Server::uuids[uuid];

                if (jsonObject.find("command") != jsonObject.end()) {
                    auto command = jsonObject["command"];
                    command["sender"] = from->uuid;
                    std::cout << "Sending [" << to->uuid << "] a message." << std::endl;
                    to->send(command.dump());
                }

            }
            else {
                std::cout << "Couldn't find uuid: " << uuid << "." << std::endl;
                from->send("{\"request\":\"error\", \"error\" : \"UUIDIsNotAvailable\", \"sender\":\"Ambassador\" }\n");
            }

        }
        else {
            // or redirect to everyone

        }
    } catch (std::invalid_argument error) {
        std::cout << "Error while parsing: " << error.what() << std::endl;

//        for (auto pair: Server::clients) {
//            Client *client = pair.second;
//            if (client != from) { // do not send me same message back
//                std::string cargo = "[" + from->uuid + "]: " + message + "\n";
//                client->send(cargo);
//            }
//        }

    }


}

void Handler::onUUID(Client *from, std::string message) {
    if (Server::uuids.count(message)) {
        std::cout << "There is already someone with that uuid for: " << message;
        std::cout << " in " << from->fd << "." << std::endl;

        from->send("There is already someone with that uuid.\n");
        Server::closeClient(from);
    }


    Server::uuids[message] = from;
    from->uuid = message;
    std::cout << "Client from " << from->fd << ", now is " << from->uuid << "." << std::endl;
    from->status = Client::Status::CHAT;
}

Handler::~Handler() {

}


