#include "Server.h"
#include "Protocol.h"
#include <string>
#include <iostream>
#include <sstream>

#define LISTEN_BACKLOG 50

Server::Server(const char *service) {
    this->sk.bind_and_listen(NULL, service, LISTEN_BACKLOG);
}

void Server::run() {
    while (1) {
        Socket peer_sk;
        this->sk.accept(peer_sk);
        this->threads.push_back(ServerThread(peer_sk, queuesMap));
        this->threads.back().start();
    }
    
}
