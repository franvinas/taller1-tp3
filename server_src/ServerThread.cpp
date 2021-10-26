#include "ServerThread.h"
#include <iostream>

ServerThread::ServerThread(Socket &peer_sk, QueuesMap &queuesMap) 
                : protocol(peer_sk),
                queuesMap(queuesMap) {}

void ServerThread::run() {
    std::string cmd;
    std::string queue_name;
    std::string message;
    std::cout << "Nuevo thread\n";
    while (this->protocol.server_recv(cmd, queue_name, message) > 0) {
        if (cmd == "define") {
            this->queuesMap.define(queue_name);
        } else if (cmd == "push") {
            this->queuesMap.push(queue_name, message);
        } else if (cmd == "pop") {
            std::string msg = this->queuesMap.pop(queue_name);
            this->protocol.server_send(msg);
        } else {
            throw -1;
        }
    }
}
