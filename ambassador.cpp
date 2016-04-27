/*
** ambassador.cpp
*/

#include <Server.h>
#include <Constants.h>

int main(int argc, char **argv) {
    Server *server = new Server(PORT);
    server->start();
}
