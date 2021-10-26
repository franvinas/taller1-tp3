#include "server_obj.h"
#include <iostream>

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cout << "Error en la cantidad de argumentos\n"
        << "El servidor se ejecuta de la siguiente manera:\n"
        << "./server <service>\n";
        return 1;
    }
    

    try {
        Server server(argv[1]);
        server.run();
    } catch(...) {
        return 2;
    }


    return 0;
}
