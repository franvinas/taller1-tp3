#include "Proxy.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

#define LISTEN_BACKLOG 50

/***********************
    Metodos privados
************************/

void Proxy::send_string(Socket &sk, std::string &str) {
    const char *str_to_send = str.c_str();
    unsigned short int str_len = str.length();
    unsigned short int str_len_be = htons(str_len);
    char str_len_to_send[2];
    memcpy(str_len_to_send, &str_len_be, 2);
    sk.send(str_len_to_send, 2);
    sk.send(str_to_send, str_len);
}

std::string Proxy::recv_string(Socket &sk) {
    std::string str;
    char name_len_msg[2];
    int b = sk.recv(name_len_msg, 2);
    if (b <= 0) throw -1;
    unsigned short int name_len_be;
    memcpy(&name_len_be, name_len_msg, 2);
    unsigned short int name_len = ntohs(name_len_be);

    char *name = (char *) malloc(name_len);
    b = sk.recv(name, name_len);
    if (b <= 0) throw -1;
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

// void Proxy::free_client() {
//     this->peer_sk.close();
// }

void Proxy::connect(const char *host, const char *port) {
    this->sk.connect(host, port);
}

void Proxy::server_send(std::string &msg) {
    send_string(this->peer_sk, msg);
}

int Proxy::server_recv(std::string &cmd, 
                       std::string &queue_name, 
                       std::string &message) {
    char cmd_char;
    int b = this->peer_sk.recv(&cmd_char, 1);
    if (b == 0) return 0; // Socket closed
    queue_name = this->recv_string(this->peer_sk);

    if (cmd_char == 'd') {
        cmd = "define";
    } else if (cmd_char == 'u') {
        cmd = "push";
        message = this->recv_string(this->peer_sk);
    } else if (cmd_char == 'o') {
        cmd = "pop";
    } else {
        throw -1;
    }
    return b;
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
    this->send_string(this->sk, queue_name);
    
    if (cmd == "push") {
        std::string message;
        cmd_unparsed_stream >> message;
        this->send_string(this->sk, message);
    }
}

std::string Proxy::client_recv() {
    return this->recv_string(this->sk);
}
