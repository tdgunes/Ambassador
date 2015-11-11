//
// Created by tdgunes on 29.10.2015.
//

#ifndef AMBASSADOR_SERVER_H
#define AMBASSADOR_SERVER_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <signal.h>
#include <iostream>
#include <map>

// libevent2
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>


#include "Client.h"

class Server {

public:
    static std::map<int, Client *> clients;

    Server(unsigned int port);

    void start();

    ~Server();


private:
    unsigned int port;

    // called by libevent when there is data to read
    void static bufferedOnRead(struct bufferevent *bev, void *arg);

    // called by libevent when there is error on the underlying socket
    void static bufferedOnError(struct bufferevent *bev, short what, void *arg);

    void static onAccept(struct evconnlistener *listener, evutil_socket_t client_fd,
                         struct sockaddr *sa, int socklen, void *user_data);

    void static signalSigint(evutil_socket_t sig, short events, void *user_data);

    void static closeClient(Client *client);



};


#endif //AMBASSADOR_SERVER_H
