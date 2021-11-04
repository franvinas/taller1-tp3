#ifndef SERVER_H
#define SERVER_H

#include "../common_src/Socket.h"

class Server {
private:
    Socket sk;

public:
    /*
     *  Constructor
     */
    explicit Server(const char *service);
    /*
     *  Lanza el hilo aceptador y se queda leyendo de entrada estandar.
     *  Una vez que lee el caracter 'q', detiene la ejecucion del hilo
     *  aceptador y cierra el servidor.
     */
    void run();
};

#endif
