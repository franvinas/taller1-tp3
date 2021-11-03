#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include "../common_src/Socket.h"
#include <string>

class Protocol {
private:
    Socket sk;
    void send_string(std::string &str);
    std::string recv_string();

public:
    /*
     *  Constructor
     */
    explicit Protocol(Socket &sk);
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
    void close_connection();

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
     *  Destructor
     */
    // ~Protocol();
};

#endif
