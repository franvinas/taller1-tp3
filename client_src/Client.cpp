#include "Client.h"
#include "Protocol.h"
#include <string>
#include <iostream>
#include <sstream>

Client::Client(const char *host, const char *service) {    
    this->sk.connect(host, service);
}

void Client::run() {
    std::string cmd_unparsed;
    Protocol protocol(this->sk);
    while (std::getline(std::cin, cmd_unparsed)) {
        if (cmd_unparsed == "exit") {
            return; // exit
        }
        protocol.client_send(cmd_unparsed);
        std::stringstream cmd_stream(cmd_unparsed);
        std::string cmd_str;
        cmd_stream >> cmd_str;
        if (cmd_str == "pop") {
            std::string msg = protocol.client_recv();
            std::cout << msg << "\n";
        }
    }
}
