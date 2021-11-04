#include "ServerThread.h"
#include "../common_src/defs.h"
#include <iostream>
#include <string>
#include <utility>

/***********************
    Metodos privados
************************/

void ServerThread::exec_cmd(const std::string &cmd, 
                            const std::string &queue_name, 
                            const std::string &message) {
    if (cmd == DEFINE_CMD) {
        this->queuesMap.define(queue_name);
    } else if (cmd == PUSH_CMD) {
        this->queuesMap.push(queue_name, message);
    } else if (cmd == POP_CMD) {
        std::string msg = this->queuesMap.pop(queue_name);
        this->protocol.send(msg);
    } else {
        throw std::runtime_error("Command unknown");
    }
}

/***********************
    Metodos protegidos
************************/

void ServerThread::run() {
    std::string cmd, queue_name, message;
    size_t bytes_recv = this->protocol.recv(cmd, queue_name, message);
    while (this->keep_talking && bytes_recv > 0) {
        exec_cmd(cmd, queue_name, message);
        bytes_recv = this->protocol.recv(cmd, queue_name, message);
    }
    is_running = false;
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

void ServerThread::stop() {
    this->keep_talking = false;
    this->protocol.close_connection();
}

bool ServerThread::is_dead() {
    return !this->is_running;
}
