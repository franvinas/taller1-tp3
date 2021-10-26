#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../common_src/Socket.h"
#include <string>

class Protocol {
private:
    Socket &sk;
    void send_string(std::string &str);
    std::string recv_string();

public:
    /*
     *  Constructor
     */
    explicit Protocol(Socket &sk);

    /*
    *  Se debe llamar desde el cliente para conectarse a un servidor dado 
    *  el 'host' y 'port' indicados.
    */
    void connect(const char *host, const char *port);

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
    // ~Protocol();
};

#endif
