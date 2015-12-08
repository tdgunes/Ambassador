//
// Created by tdgunes on 29.10.2015.
//

#include "Server.h"
#include <vector>

std::map<int, Client *> Server::clients;
std::map<std::string, Client *> Server::uuids;

Handler *Server::handler = nullptr;

Server::Server(unsigned int port) {
    std::cout << "Server is initialising..." << std::endl;
    Server::handler = new Handler();
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


    listener = evconnlistener_new_bind(evbase, &Server::onAccept,
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
    Client *from = (Client *) arg;
    std::vector<std::string> buffer;


//    std::vector<char> message;
    /* Read 8kb at a time and send it to all connected clients. */
    while (true) {
        uint8_t data[8192]; //FIXME: protocol demands
        size_t n = 0;

        n = bufferevent_read(bev, data, sizeof(data));

        std::string message(data, data + n);

        if (message == "") {
            std::string package = "";
            for (auto token : buffer) {
                package += token;
            }

            Server::handler->handleMessage(from, package);
            break;
        }

        buffer.push_back(message);
    }


}

void Server::bufferedOnError(struct bufferevent *bev, short what, void *arg) {
    Client *client = (Client *) arg;

    if (what & BEV_EVENT_EOF) {
        /* Client disconnected, remove the read event and the
         * free the client structure. */
        printf("Client disconnected.\n");
    }
    else {
        warn("Client socket error, disconnecting.\n");
    }

    closeClient(client);
}

void Server::onAccept(struct evconnlistener *listener, evutil_socket_t client_fd,
                      struct sockaddr *sa, int socklen, void *user_data) {

    struct event_base *evbase = (event_base *) user_data;

    struct sockaddr_in *sai;


    sai = (struct sockaddr_in *) sa;

    /* We've accepted a new client, create a client object. */
    Client *client = new Client(client_fd);
    client->status = Client::Status::UUID;
    client->bufferedEvent = bufferevent_socket_new(evbase, client_fd, BEV_OPT_CLOSE_ON_FREE);

    if (client->bufferedEvent == NULL) {
        std::cout << "Unable to create socket for " << client->fd << std::endl;
    }

    if (!client->bufferedEvent) {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(evbase);
        return;
    }

    bufferevent_setcb(client->bufferedEvent, bufferedOnRead, NULL,
                      bufferedOnError, (void *) client);

    /* We have to enable it before our callbacks will be
     * called. */
    bufferevent_enable(client->bufferedEvent, EV_READ);


    clients[client->fd] = client;


    printf("Accepted connection from %s\n", inet_ntoa(sai->sin_addr));


}

void Server::signalSigint(evutil_socket_t sig, short events, void *user_data) {

    struct event_base *evbase = (struct event_base *) user_data;

    std::cout << "Caught an interrupt signal, close all clients\n" << std::endl;
    const char *goodbyeMessage = "Goodbye";

    for (auto pair : Server::clients) {
        Client *client = pair.second;
        bufferevent_write(client->bufferedEvent, goodbyeMessage, strlen(goodbyeMessage));
        bufferevent_flush(client->bufferedEvent, EV_READ | EV_WRITE, BEV_FLUSH);
        closeClient(client);
    }

    event_base_loopexit(evbase, NULL);
}

void Server::closeClient(Client *client) {

    if (client->status == Client::Status::CHAT) {
        std::cout << "Remove " << client->uuid << " from " << client->fd << "." << std::endl;
    } else {
        std::cout << "Remove " << "Anonymous" << " from " << client->fd << "." << std::endl;
    }

    bufferevent_free(client->bufferedEvent);

    close(client->fd);
    uuids.erase(client->uuid);
    clients.erase(client->fd);
    delete client;
}

Server::~Server() {
    std::cout << "Shutting down the server." << std::endl;
    delete Server::handler;
}