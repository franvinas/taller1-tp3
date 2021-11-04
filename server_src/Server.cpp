#include "Server.h"
#include "Protocol.h"
#include "AcceptorThread.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>

#define LISTEN_BACKLOG 50
#define QUIT_CHAR 'q'

/***********************
    Metodos publicos
************************/

Server::Server(const char *service) {
    this->sk.bind_and_listen(NULL, service, LISTEN_BACKLOG);
}

void Server::run() {
    AcceptorThread acceptor(this->sk);
    acceptor.start();
    char c = getchar();
    while (c != QUIT_CHAR) {
        c = std::cin.get();
    }
    acceptor.stop();
    acceptor.join();
}
