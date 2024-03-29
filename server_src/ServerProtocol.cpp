#include "ServerProtocol.h"
#include "../common_src/defs.h"
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#define LISTEN_BACKLOG 50

/***********************
    Metodos privados
************************/

void ServerProtocol::send_string(std::string &str) {
    const char *str_to_send = str.c_str();
    unsigned short int str_len = str.length();
    unsigned short int str_len_be = htons(str_len);
    char str_len_to_send[2];
    memcpy(str_len_to_send, &str_len_be, 2);
    this->sk.send(str_len_to_send, 2);
    this->sk.send(str_to_send, str_len);
}

std::string ServerProtocol::recv_string() {
    char name_len_msg[2];
    int b = this->sk.recv(name_len_msg, 2);
    if (b <= 0) throw std::runtime_error("Error in recv()");
    unsigned short int name_len_be;
    memcpy(&name_len_be, name_len_msg, 2);
    unsigned short int name_len = ntohs(name_len_be);

    std::vector<char> name;
    name.reserve(name_len);
    char *buf = &name[0];
    b = this->sk.recv(buf, name_len);
    if (b <= 0) throw std::runtime_error("Error in recv()");
    return std::string(buf, name_len);
}

std::string ServerProtocol::parse_cmd(char c) {
    if (c == DEFINE_CHAR) {
        return DEFINE_CMD;
    } else if (c == PUSH_CHAR) {
        return PUSH_CMD;
    } else if (c == POP_CHAR) {
        return POP_CMD;
    }
    throw std::runtime_error("Command unknown");
}

/***********************
    Metodos publicos
************************/

ServerProtocol::ServerProtocol(Socket &sk) : sk(std::move(sk)) {}


void ServerProtocol::send(std::string &msg) {
    send_string(msg);
}

int ServerProtocol::recv(std::string &cmd, 
                   std::string &queue_name, 
                   std::string &message) {
    char cmd_char;
    int b = this->sk.recv(&cmd_char, 1);
    if (b == -1) throw std::runtime_error("Error in recv()");
    if (b == 0) return 0; // Socket closed
    queue_name = this->recv_string();
    cmd = parse_cmd(cmd_char);

    if (cmd == PUSH_CMD)
        message = this->recv_string();
    
    return b;
}

void ServerProtocol::close_connection() {
    this->sk.shutdown();
    this->sk.close();
}
