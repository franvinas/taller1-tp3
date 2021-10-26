#ifndef PROXY_H
#define PROXY_H

#include "Socket.h"
#include <string>

class Proxy {
private:
    Socket sk;
    Socket peer_sk;
    void send_string(Socket &sk, std::string &str);
    std::string recv_string(Socket &sk);

public:
    /*
     *  Constructor
     */
    Proxy();
    /*
    *  Se debe llamar desde el servidor para esperar un cliente.
    */
    void listen(const char *port);

    /*
    *  Acepta un nuevo cliente.
    */
    void new_client();

    /*
    *  Termina la conexión con el cliente.
    */
    // void free_client();

    /*
    *  Se debe llamar desde el cliente para conectarse a un servidor dado 
    *  el 'host' y 'port' indicados.
    */
    void connect(const char *host, const char *port);

    /*
    *  Se debe llamar desde el servidor para enviar un mensaje al cliente con el
    *  protocolo (manejo de bits) adecuado.
    */
    void server_send(std::string &msg);

    /*
    *  Se debe llamar desde el servidor.
    */
    int server_recv(std::string &cmd, 
                    std::string &queue_name, 
                    std::string &message);

    /*
    *  Se debe llamar desde el cliente.
    */
    void client_send(std::string &cmd_unparsed);

    /*
    *  Se debe llamar desde el cliente luego de un pop para recibir el mensaje
    */
    std::string client_recv();
    /*
     *  Destructor
     */
    // ~Proxy();
};

#endif
