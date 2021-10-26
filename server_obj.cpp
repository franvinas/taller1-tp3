#include "server_obj.h"
#include "common_command.h"
#include <string>
#include <iostream>
#include <sstream>

Server::Server(const char *service) {
    proxy.listen(service);
}

void Server::run() {
    proxy.new_client();
    Command *command;
    while ((command = proxy.server_recv()) != nullptr) {
        command->operate();
        delete command;
    }
    

}
