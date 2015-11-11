/*
** ambassador.c
*/

#include <Server.h>

#define PORT 5554   // port we're listening on

int main(int argc, char **argv) {
    Server *server = new Server(PORT);
    server->start();
}
