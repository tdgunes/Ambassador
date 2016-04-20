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

void Handler::handleMessage(Client *from, std::string &message) {
    if (from == nullptr)
        throw std::invalid_argument("Received a null client as parameter.");

    message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
    message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
//    std::cout << "[>>>\n" << message << "\n<<<]" << std::endl;
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


void Handler::onChat(Client *from, std::string &message) {



    try {
        auto jsonObject = json::parse(message);
        // redirect message protocol, {"uuid": receiver, "command": package}
        if (jsonObject.find("uuid") != jsonObject.end() && jsonObject.find("command") != jsonObject.end()) {
            std::string uuid = jsonObject["uuid"];

            if (uuid.length() > 0) {
                if (Server::uuids.find(uuid) != Server::uuids.end()) {
                    Client *to = Server::uuids[uuid];

                    auto command = jsonObject["command"];
                    command["sender"] = from->uuid;
                    std::cout << "Sending [" << to->uuid << "] a message." << std::endl;
                    to->send(command.dump());

                }
                else {
                    std::cout << "Couldn't find uuid: " << uuid << "." << std::endl;
                    std::string error = "{\"category\":\"error\", "
                            "\"error\" : \"Device that is requested, is not connected to local Ambassador. Please make sure that your device is online and connected!\", "
                            "\"type\" : \"DeviceNotConnected\", "
                            "\"sender\":\"Ambassador\"}\n";
                    from->send(error);
                }

            }
            else {
                //FIXME: or maybe redirect to everyone (dangerous!)

            }
        }
            // event system protocol, {"listen": true, "follow":[uuid1, uuid2, uuid3, ... ] }
        else if (jsonObject.find("listen") != jsonObject.end() &&
                 jsonObject.find("follow") != jsonObject.end() &&
                 jsonObject["follow"].is_array()) {

            from->unfollowAll(); //FIXME: only changes in follow list can be added here, instead of resetting
            for (auto &uuid: jsonObject["follow"]) {
                from->follow(uuid);
            }

            Server::eventSystem.join(from);
        }
        else {
            std::cout << "\"uuid\" or \"command\" not found inside the message package" << std::endl;
        }
    } catch (std::invalid_argument error) {
        std::cout << "Error while parsing: " << error.what() << std::endl;



    }


}

void Handler::onUUID(Client *from, std::string &message) {
    if (Server::uuids.count(message)) {
        std::cout << "There is already someone with that uuid for: " << message;
        std::cout << " in " << from->fd << "." << std::endl;
        std::cout << "Closing the other connection!" << std::endl;

        Client *previous = Server::uuids[message];
        Server::closeClient(previous);
    }


    Server::uuids[message] = from;
    from->uuid = message;
    std::cout << "Client from " << from->fd << ", now is " << from->uuid << "." << std::endl;
    Server::eventSystem.update(EventSystem::Event::CONNECTED, from);
    from->status = Client::Status::CHAT;
}

Handler::~Handler() {

}


