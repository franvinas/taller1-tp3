#ifndef SOCKET_H
#define SOCKET_H

#include <string>

class Socket {
private:
    int fd;
    int _getaddrinfo(const char *host,
                    const char *service,
                    struct addrinfo **ptr,
                    bool passive);
    int _reuse_address();
    int _bind(struct addrinfo *ptr);
    int _listen(int queue_length);
    void _accept(int &fd);

public:
    /*
     *  Constructor
     */
    Socket();
    /*
     *  Constructor 2
     */
    Socket(int &fd);
    /*
    *  Asigna IP ("host") y numero de puerto ("service") al socket.
    *  "queue_length" indica la cantidad de cliente que se quiere mantener en
    *  espera. Se atiende a un cliente a la vez.
    */
    int bind_and_listen(const char *host, 
                        const char *service, 
                        int queue_length);
    /*
    *  Acepta al socket "peer". Una vez que la conexión finaliza
    *  el socket peer debe ser destruido.
    */
    void accept(Socket &peer);

    /*
    *  El socket se conecta la IP dada por "host" en el puerto dado por "service".
    */
    int connect(const char *host, const char *service);

    /*
    *  Se envian una cantidad de bytes (dada por len) del "buffer".
    *  Se asegura que todos los bytes se envian.
    *  Si ocurre un error la funcion retorna un valor distinto de 0.
    */
    int send(const char *buffer, ssize_t len);

    /*
    *  Se recibe una cantidad de bytes (dada por len) y se guarda en el "buffer".
    *  Se asegura que todos los bytes se reciben.
    *  Si ocurre un error la funcion retorna un valor distinto de 0.
    */
    int recv(char *buffer, ssize_t len);
    /*
     *  Destructor
     */
    ~Socket();
};

#endif