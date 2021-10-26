#include "client_obj.h"
#include <iostream>

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        std::cerr << "Error en la cantidad de argumentos\n"
        << "El cliente se ejecuta de la siguiente manera:\n"
        << "./client <host> <service>\n";
        return 1;
    }
    
    try {
        Client client(argv[1], argv[2]);
        client.run();
    } catch(...) {
        return 2;
    }
    
    return 0;
}
