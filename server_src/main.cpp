#include "Server.h"
#include <iostream>

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Error en la cantidad de argumentos\n"
        << "El servidor se ejecuta de la siguiente manera:\n"
        << "./server <service>\n";
        return 1;
    }
    
    try {
        Server server(argv[1]);
        server.run();
    } catch(const std::exception &e) {
        std::cerr << "Se atrapó una excepcion en " 
                  << "el hilo principal del servidor: '" 
                  << e.what() << "'" << std::endl;
        return 2;
    } catch(...) {
        std::cerr << "Error desconocido en el hilo " 
                  << "principal del servidor" << std::endl;
        return 2;
    }


    return 0;
}
