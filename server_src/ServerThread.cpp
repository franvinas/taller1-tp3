#include "ServerThread.h"
#include <iostream>
#include <string>

/***********************
    Metodos privados
************************/

void ServerThread::exec_cmd(const std::string &cmd, 
                            const std::string &queue_name, 
                            const std::string &message) {
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
}

/***********************
    Metodos publicos
************************/

ServerThread::ServerThread(Socket &peer_sk, QueuesMap &queuesMap) 
                : protocol(peer_sk),
                  queuesMap(queuesMap),
                  keep_talking(true),
                  is_running(true) {}

ServerThread::ServerThread(ServerThread &&other) 
                : Thread(std::move(other)),
                  protocol(std::move(other.protocol)),
                  queuesMap(other.queuesMap),
                  keep_talking(!other.is_dead()),
                  is_running(!other.is_dead()) {}

void ServerThread::run() {
    std::string cmd;
    std::string queue_name;
    std::string message;
    size_t bytes_recv = this->protocol.server_recv(cmd, queue_name, message);
    while (this->keep_talking && bytes_recv > 0) {
        exec_cmd(cmd, queue_name, message);
        bytes_recv = this->protocol.server_recv(cmd, queue_name, message);
    }
    is_running = false;
}

void ServerThread::stop() {
    this->keep_talking = false;
    this->protocol.close_connection();
}

bool ServerThread::is_dead() {
    return !this->is_running;
}
