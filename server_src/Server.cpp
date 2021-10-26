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
    this->sk.accept(this->peer_sk);
    Protocol protocol(this->peer_sk);
    std::string cmd;
    std::string queue_name;
    std::string message;
    while (protocol.server_recv(cmd, queue_name, message) > 0) {
        if (cmd == "define") {
            this->queuesMap.define(queue_name);
        } else if (cmd == "push") {
            this->queuesMap.push(queue_name, message);
        } else if (cmd == "pop") {
            std::string msg = this->queuesMap.pop(queue_name);
            protocol.server_send(msg);
        } else {
            throw -1;
        }
    }
}
