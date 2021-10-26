#include "Client.h"
#include <string>
#include <iostream>
#include <sstream>

Client::Client(const char *host, const char *service) {    
    proxy.connect(host, service);
}

void Client::run() {
    std::string cmd_unparsed;
    while (std::getline(std::cin, cmd_unparsed)) {
        if (cmd_unparsed == "exit") {
            return; // exit
        }
        proxy.client_send(cmd_unparsed);
        std::stringstream cmd_stream(cmd_unparsed);
        std::string cmd_str;
        cmd_stream >> cmd_str;
        if (cmd_str == "pop") {
            std::string msg = proxy.client_recv();
            std::cout << msg << "\n";
        }
    }
}
