#include "Socket.h"
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/***********************
    Metodos privados
************************/

int Socket::_getaddrinfo(const char *host,
                         const char *service,
                         struct addrinfo **ptr,
                         bool passive) {
    int s = 0;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;                  /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;            /* TCP  */
    hints.ai_flags = passive ? AI_PASSIVE : 0;  /* AI_PASSIVE para el server */
                                                /* y 0 para el cliente */
    s = getaddrinfo(host, service, &hints, ptr);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }
    return 0;
}

/*  Se activa la opción de reusar la direccion en caso de que esta
 *  no este disponible por un TIME_WAIT 
 */
int Socket::_reuse_address() {
    int val = 1;
    int s = setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) return -1;
    return 0;
}

int Socket::_bind(struct addrinfo *ptr) {
    int s = bind(this->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int Socket::_listen(const int &queue_length) {
    int s = listen(this->fd, queue_length);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(this->fd);
        return -1;
    }
    return 0;
}

void Socket::_accept(const int &fd) {
    this->fd = fd;
}

/***********************
    Metodos publicos
************************/

Socket::Socket() : fd(-1) {}

Socket::Socket(int &fd) : fd(std::move(fd)) {}

Socket::~Socket() {
    if (this->fd != -1)
        close(this->fd);
}

int Socket::bind_and_listen(const char *host,
                            const char *service,
                            int queue_length) {
    struct addrinfo *ptr, *rp;
    bool passive = true; // necesario para luego aceptar conexiones
    
    if (this->_getaddrinfo(host, service, &ptr, passive) != 0)
        return -1;

    for (rp = ptr; rp != NULL; rp = rp->ai_next) {
        this->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (this->fd == -1)
            continue;

        if (this->_bind(rp) == 0)
            break; // Bind exitoso

        close(this->fd);
    }
    freeaddrinfo(ptr);
    
    if (rp == NULL) { // Falló bind para todas las addrs
        printf("Error: %s\n", strerror(errno));
        close(this->fd);
        return -1;
    }

    if (this->_reuse_address() != 0) {
        printf("Error: %s\n", strerror(errno));
        close(this->fd);
        return -1;
    }
    
    if (this->_listen(queue_length) != 0) {
        printf("Error: %s\n", strerror(errno));
        close(this->fd);
        return -1;
    }
    return 0;
}

void Socket::accept(Socket &peer) {
    int a = ::accept(this->fd, NULL, NULL);
    if (a == -1) {
        printf("Error: %s\n", strerror(errno));
        throw -1;
    }
    peer._accept(a);
}

int Socket::connect(const char *host, const char *service) {
    struct addrinfo *ptr, *rp;
    bool passive = false; // necesario para luego usar connect
    
    if (this->_getaddrinfo(host, service, &ptr, passive) != 0)
        return -1;

    for (rp = ptr; rp != NULL; rp = rp->ai_next) {
        this->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (this->fd == -1)
            continue;

        if (::connect(this->fd, ptr->ai_addr, ptr->ai_addrlen) == 0)
            break; // Connect exitoso

        close(this->fd);
    }
    freeaddrinfo(ptr);
    
    if (rp == NULL) { // Falló connect para todas las addrs
        printf("Error: %s\n", strerror(errno));
        close(this->fd);
        return -1;
    }

    return 0;
}

int Socket::send(const char *buffer, ssize_t len) {
    ssize_t sent_b = 0;
    while (len > sent_b) {
        ssize_t b = ::send(this->fd, buffer + sent_b, 
                            len - sent_b, MSG_NOSIGNAL);
        if (b == -1) {
            printf("Error send: %s\n", strerror(errno));
            return -1;
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            sent_b += b;
        }
    }
    return 0;
}

int Socket::recv(char *buffer, ssize_t len) {
    ssize_t recv_b = 0;

    while (len > recv_b) {
        ssize_t b = ::recv(this->fd, buffer + recv_b, len - recv_b, 0);
        if (b == -1) {
            printf("Error recv: %s\n", strerror(errno));
            return -1;
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            recv_b += b;
        }
    }
    return recv_b;
}
