#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../common_src/Socket.h"
#include <string>

class Protocol {
private:
    Socket &sk;
    void send_string(std::string &str);
    std::string recv_string();
    char parse_cmd(const std::string &cmd);

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
     *  Le envia al servidor el comando con el protocolo de mensaje adecuado
     */
    void send(std::string &cmd_unparsed);

    /*
     *  Recibe un mensaje del servidor
     */
    std::string recv();
};

#endif
