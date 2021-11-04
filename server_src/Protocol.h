#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include "../common_src/Socket.h"
#include <string>

class Protocol {
private:
    Socket sk;
    void send_string(std::string &str);
    std::string recv_string();
    std::string parse_cmd(char c);

public:
    /*
     *  Constructor
     */
    explicit Protocol(Socket &sk);
    /*
     *  Esperar a que un cliente se conecte.
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
     *  Envia un mensaje al cliente con el
     *  protocolo (manejo de bits) adecuado.
     */
    void send(std::string &msg);

    /*
     *  Recibe un comando de un cliente.
     */
    int recv(std::string &cmd, 
             std::string &queue_name, 
             std::string &message);
};

#endif
