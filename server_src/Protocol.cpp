#include "Protocol.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>

#define LISTEN_BACKLOG 50

/***********************
    Metodos privados
************************/

void Protocol::send_string(std::string &str) {
    const char *str_to_send = str.c_str();
    unsigned short int str_len = str.length();
    unsigned short int str_len_be = htons(str_len);
    char str_len_to_send[2];
    memcpy(str_len_to_send, &str_len_be, 2);
    this->sk.send(str_len_to_send, 2);
    this->sk.send(str_to_send, str_len);
}

std::string Protocol::recv_string() {
    std::string str;
    char name_len_msg[2];
    int b = this->sk.recv(name_len_msg, 2);
    if (b <= 0) throw std::runtime_error("Error in recv()");
    unsigned short int name_len_be;
    memcpy(&name_len_be, name_len_msg, 2);
    unsigned short int name_len = ntohs(name_len_be);

    char *name = (char *) malloc(name_len);
    b = this->sk.recv(name, name_len);
    if (b <= 0) throw std::runtime_error("Error in recv()");
    str.clear();
    str.append(name, name_len);
    free(name);
    return str;
}

/***********************
    Metodos publicos
************************/

Protocol::Protocol(Socket &sk) : sk(std::move(sk)) {}


void Protocol::server_send(std::string &msg) {
    send_string(msg);
}

int Protocol::server_recv(std::string &cmd, 
                          std::string &queue_name, 
                          std::string &message) {
    char cmd_char;
    int b = this->sk.recv(&cmd_char, 1);
    if (b == -1) throw std::runtime_error("Error in recv()");
    if (b == 0) return 0; // Socket closed
    queue_name = this->recv_string();

    if (cmd_char == 'd') {
        cmd = "define";
    } else if (cmd_char == 'u') {
        cmd = "push";
        message = this->recv_string();
    } else if (cmd_char == 'o') {
        cmd = "pop";
    } else {
        throw std::runtime_error("Command unknown");
    }
    return b;
}

void Protocol::close_connection() {
    this->sk.shutdown();
    this->sk.close();
}
