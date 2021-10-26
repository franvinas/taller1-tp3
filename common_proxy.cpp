#include "common_proxy.h"
#include "common_command.h"
#include <string.h>
#include <sstream>
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>

#define LISTEN_BACKLOG 50

/***********************
    Metodos privados
************************/

void Proxy::send_string(std::string &str) {
    const char *str_to_send = str.c_str();
    unsigned short int str_len = str.length();
    unsigned short int str_len_be = htons(str_len);
    char str_len_to_send[2];
    memcpy(str_len_to_send, &str_len_be, 2);
    this->sk.send(str_len_to_send, 2);
    this->sk.send(str_to_send, str_len);
}

std::string Proxy::recv_string() {
    std::string str;
    char name_len_msg[2];
    int b = this->peer_sk.recv(name_len_msg, 2);
    if (b <= 0) throw -1;
    unsigned short int name_len_be;
    memcpy(&name_len_be, name_len_msg, 2);
    unsigned short int name_len = ntohs(name_len_be);

    char *name = (char *) malloc(name_len);
    b = this->peer_sk.recv(name, name_len);
    str.clear();
    str.append(name, name_len);
    free(name);
    return str;
}

/***********************
    Metodos publicos
************************/

Proxy::Proxy() {}

void Proxy::listen(const char *port) {
    this->sk.bind_and_listen(NULL, port, LISTEN_BACKLOG);
}

void Proxy::new_client() {
    this->sk.accept(this->peer_sk);
}

void Proxy::free_client() {
    // this->peer_sk.close();
}

void Proxy::connect(const char *host, const char *port) {
    this->sk.connect(host, port);
}

void Proxy::server_send(const char *msg) {
    // int s = socket_send(&self->peer_sk, msg, msg_len);
}

Command *Proxy::server_recv() {
    char cmd;
    int b = this->peer_sk.recv(&cmd, 1);
    if (b == 0) return nullptr; // Socket closed
    std::string queue_name = this->recv_string();

    if (cmd == 'd') {
        return new Define(queue_name);
    } else if (cmd == 'u') {
        std::string message = this->recv_string();
        return new Push(queue_name, message);
    } else if (cmd == 'o') {
        return new Pop(queue_name);
    } else {
        throw -1;
    }
}

void Proxy::client_send(std::string &cmd_unparsed) {
    std::stringstream cmd_unparsed_stream(cmd_unparsed);
    std::string cmd;
    char cmd_char;
    cmd_unparsed_stream >> cmd;
    if (cmd == "define") {
        cmd_char = 'd';
    } else if (cmd == "push") {
        cmd_char = 'u';
    } else if (cmd == "pop") {
        cmd_char = 'o';
    } else {
        throw -1;
    }
    this->sk.send(&cmd_char, 1);
    std::string queue_name;
    cmd_unparsed_stream >> queue_name;
    this->send_string(queue_name);
    
    if (cmd == "push") {
        std::string message;
        cmd_unparsed_stream >> message;
        this->send_string(message);
    }
}

std::string Proxy::client_recv() {
    return "hola";
}

// void Proxy::client_send(std::string cmd_unparsed) {
//     std::stringstream cmd_unparsed_stream(cmd_unparsed);
//     std::string cmd;
//     cmd_unparsed_stream >> cmd;
//     if (cmd == "define") {
//         std::cout << "Define\n";
//     } else if (cmd == "push") {
//         std::cout << "Push\n";
//     } else if (cmd == "pop") {
//         std::cout << "POP\n";
//     } else if (cmd == "exit") {
//         std::cout << "EXIT\n";
//     } else {
//         std::cout << "Comando no reconocido\n";
//     }
// }

