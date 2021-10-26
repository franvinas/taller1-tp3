#ifndef PROXY_H
#define PROXY_H

#include "common_socket.h"
#include "common_command.h"
#include <string>

class Proxy {
private:
    Socket sk;
    Socket peer_sk;
    void send_string(std::string &str);
    std::string recv_string();

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
    void free_client();

    /*
    *  Se debe llamar desde el cliente para conectarse a un servidor dado 
    *  el 'host' y 'port' indicados.
    */
    void connect(const char *host, const char *port);

    /*
    *  Se debe llamar desde el servidor para enviar un mensaje al cliente con el
    *  protocolo (manejo de bits) adecuado.
    */
    void server_send(const char *game_state);

    /*
    *  Se debe llamar desde el servidor. Recibe un caracter 'c'.
    */
    Command *server_recv();

    /*
    *  Se debe llamar desde el cliente. Envía un caracter 'c'.
    */
    void client_send(std::string &cmd_unparsed);

    /*
    *  Se debe llamar desde el cliente para recibir un mensaje al servidor con el
    *  protocolo (manejo de bits) adecuado. Se almacenan los datos extraídos del 
    *  mensaje recibido en 'partial_word', 'tries_left' y 'game_over'.
    *  Si 'partial_word' es un puntero a NULL se aloca la memoria necesaria. Es 
    *  responsabilidad del usuario de la función liberar esta memoria.
    */
    std::string client_recv();
    /*
     *  Destructor
     */
    // ~Proxy();
};

#endif