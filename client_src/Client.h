#ifndef CLIENT_H
#define CLIENT_H

#include "../common_src/Socket.h"

class Client {
private:
    Socket sk;
    void define();
    void push();
    void pop();

public:
    /*
     *  Constructor
     */
    Client(const char *host, const char *service);
    /*
     *  Corre el cliente. Su ejecucion consiste en leer comandos de entrada 
     *  estandar, parsearlos y enviarlos al servidor.
     */
    void run();
};

#endif
