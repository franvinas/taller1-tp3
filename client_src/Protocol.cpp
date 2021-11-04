#include "Protocol.h"
#include "../common_src/defs.h"
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

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

char Protocol::parse_cmd(const std::string &cmd) {
    if (cmd == DEFINE_CMD) {
        return DEFINE_CHAR;
    } else if (cmd == PUSH_CMD) {
        return PUSH_CHAR;
    } else if (cmd == POP_CMD) {
        return POP_CHAR;
    }
    throw std::runtime_error("Command unknown");
}

/***********************
    Metodos publicos
************************/

Protocol::Protocol(Socket &sk) : sk(sk) {}

void Protocol::connect(const char *host, const char *port) {
    this->sk.connect(host, port);
}

void Protocol::send(std::string &cmd_unparsed) {
    std::stringstream cmd_unparsed_stream(cmd_unparsed);
    std::string cmd, queue_name;
    cmd_unparsed_stream >> cmd;
    
    char cmd_char = parse_cmd(cmd);
    this->sk.send(&cmd_char, 1);
    
    cmd_unparsed_stream >> queue_name;
    this->send_string(queue_name);
    
    if (cmd == PUSH_CMD) {
        std::string message;
        cmd_unparsed_stream >> message;
        this->send_string(message);
    }
}

std::string Protocol::recv() {
    return this->recv_string();
}
