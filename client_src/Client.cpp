#include "Client.h"
#include "ClientProtocol.h"
#include "../common_src/defs.h"
#include <string>
#include <iostream>
#include <sstream>

#define EXIT_CMD "exit"

/***********************
    Metodos publicos
************************/

Client::Client(const char *host, const char *service) {
    this->protocol.connect(host, service);
}

void Client::run() {
    std::string cmd_unparsed;
    while (std::getline(std::cin, cmd_unparsed)) {
        if (cmd_unparsed == EXIT_CMD) break;
        if (protocol.send(cmd_unparsed) != 0) break; // Closed server
        std::stringstream cmd_stream(cmd_unparsed);
        std::string cmd_str;
        cmd_stream >> cmd_str;
        if (cmd_str == POP_CMD) {
            std::string msg = protocol.recv();
            std::cout << msg << "\n";
        }
    }
}
