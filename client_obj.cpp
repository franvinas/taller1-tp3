#include "client_obj.h"
#include <string>
#include <iostream>

Client::Client(const char *host, const char *service) {    
    proxy.connect(host, service);
}

void Client::run() {
    std::string cmd_unparsed;
    while(std::getline (std::cin, cmd_unparsed)) {
        if (cmd_unparsed == "exit") {
            return; // exit
        }
        proxy.client_send(cmd_unparsed);
    }

}
