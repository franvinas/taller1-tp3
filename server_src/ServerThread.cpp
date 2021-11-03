#include "ServerThread.h"
#include <iostream>
#include <string>

ServerThread::ServerThread(Socket &peer_sk, QueuesMap &queuesMap) 
                : protocol(peer_sk),
                  queuesMap(queuesMap),
                  keep_talking(true),
                  is_running(false) {}

ServerThread::ServerThread(ServerThread &&other) 
                : Thread(std::move(other)),
                  protocol(std::move(other.protocol)),
                  queuesMap(other.queuesMap) {}

void ServerThread::run() {
    std::string cmd;
    std::string queue_name;
    std::string message;
    is_running = true;
    keep_talking = true;
    size_t bytes_recv = this->protocol.server_recv(cmd, queue_name, message);
    while (this->keep_talking && bytes_recv > 0) {
        if (cmd == "define") {
            this->queuesMap.define(queue_name);
        } else if (cmd == "push") {
            this->queuesMap.push(queue_name, message);
        } else if (cmd == "pop") {
            std::string msg = this->queuesMap.pop(queue_name);
            this->protocol.server_send(msg);
        } else {
            throw std::runtime_error("Command unknown");
        }
        bytes_recv = this->protocol.server_recv(cmd, queue_name, message);
    }
    is_running = false;
}

void ServerThread::stop() {
    this->keep_talking = false;
    this->protocol.close_connection();
}
