//
// Created by tdgunes on 29.10.2015.
//

#include "Server.h"
#include <iostream>

Server::Server() {
    std::cout << "Server is initialising..." << std::endl;
}

Server::~Server() {
    std::cout << "Shutting down the server." << std::endl;
}
