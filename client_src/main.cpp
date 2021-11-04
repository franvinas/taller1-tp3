#include "Client.h"
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
    } catch(const std::exception &e) {
        std::cerr << "Se atrapó una excepcion en el cliente: '" 
                  << e.what() << "'" << std::endl;
        return 2;
    } catch(...) {
        std::cerr << "Error desconocido en el cliente" << std::endl;
        return 2;
    }

    return 0;
}
