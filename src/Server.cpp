//
// Created by tdgunes on 29.10.2015.
//



#include "Server.h"

std::map<int, Client *> Server::clients;

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
    Client *thisClient = (Client *) arg;
    uint8_t data[8192];
    size_t n = 0;

    /* Read 8k at a time and send it to all connected clients. */
    while (true) {
        n = bufferevent_read(bev, data, sizeof(data));
        if (n <= 0) {
            break;
        }

        if (thisClient->status == Client::Status::CHAT) {
            std::cout << "[" << thisClient->nickname << "]: " << data;
            for (auto pair: clients) {
                Client *client = pair.second;
                if (client != thisClient) {
                    bufferevent_write(client->bufferedEvent, "[", 2);
                    bufferevent_write(client->bufferedEvent,
                                      thisClient->nickname.c_str(),
                                      strlen(thisClient->nickname.c_str()));
                    bufferevent_write(client->bufferedEvent, "]: ", 4);
                    bufferevent_write(client->bufferedEvent, data, n);
                }
            }
        }

        if (thisClient->status == Client::Status::NICKNAME) {
            std::string nickname(data, data + n - 2);
            thisClient->nickname = nickname;


            std::cout << "Client from " << thisClient->fd << ", now is " << nickname << "." << std::endl;
            thisClient->status = Client::Status::CHAT;
        }

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
    const char welcome_message[] = "Nickname: ";
    struct sockaddr_in *sai;


    sai = (struct sockaddr_in *) sa;

    /* We've accepted a new client, create a client object. */
    Client *client = new Client(client_fd);
    client->status = Client::Status::NICKNAME;
    client->bufferedEvent = bufferevent_socket_new(evbase, client_fd, BEV_OPT_CLOSE_ON_FREE);

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

    bufferevent_write(client->bufferedEvent, welcome_message, sizeof(welcome_message));
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

    if (client->status == Client::Status::NICKNAME) {
        std::cout << "Remove " << client->nickname << "from" << client->fd << std::endl;
    } else {
        std::cout << "Remove " << "Anonymous" << " from " << client->fd << std::endl;
    }

    bufferevent_free(client->bufferedEvent);
    close(client->fd);
    clients.erase(client->fd);
    delete client;
}



Server::~Server() {
    std::cout << "Shutting down the server." << std::endl;
}



