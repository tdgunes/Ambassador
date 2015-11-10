//
// Created by tdgunes on 29.10.2015.
//



#include "Server.h"


Server::Server(unsigned int port) {
    std::cout << "Server is initialising..." << std::endl;
    this->port = port;
}

void Server::start() {
    struct evconnlistener *listener;
    struct sockaddr_in sin;
    struct event_base *evbase;
    struct event *signalEvent;

    // Initializing libevent

    std::cout << "Initialize event_base." << std::endl;

    evbase = event_base_new();
    if (evbase == 0) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(this->port);

    std::cout << "Bind port " << this->port << std::endl;


    listener = evconnlistener_new_bind(evbase,
                                       onAccept,
                                       (void *) evbase,
                                       LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                       -1,
                                       (struct sockaddr *) &sin,
                                       sizeof(sin));

    if (listener == 0) {
        fprintf(stderr, "Could not create a listener!\n");
        return;
    }

    printf("Program signal SIGINT.\n");

    signalEvent = evsignal_new(evbase, SIGINT, signalSigint, (void *) evbase);

    if (!signalEvent || event_add(signalEvent, NULL) < 0) {
        fprintf(stderr, "Could not create/add a signal event!\n");
        return;
    }

    /* Start the event loop. */

    printf("Start event loop - Waiting for connections.\n");

    event_base_dispatch(evbase);

    /* When stop loop */

    printf("End of Loop.\n");

    evconnlistener_free(listener);
    event_free(signalEvent);
    event_base_free(evbase);
}


void Server::bufferedOnRead(struct bufferevent *bev, void *arg) {

}

void Server::bufferedOnError(struct bufferevent *bev, void *arg) {

}

void Server::onAccept(struct evconnlistener *listener, evutil_socket_t client_fd,
                      struct sockaddr *sa, int socklen, void *user_data) {

}

void static Server::signalSigint(evutil_socket_t sig, short events, void *user_data) {

}

void Server::closeClient(Client *client) {

}



Server::~Server() {
    std::cout << "Shutting down the server." << std::endl;
}



